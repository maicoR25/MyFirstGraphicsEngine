#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <functional>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "gui_panel.h"

class GUIManager {
public:
	Scene* m_activeScene = nullptr;

	// Initializes the ImGUI context
	GUIManager(GLFWwindow* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();
	}

	// Destroys ImGUI context
	~GUIManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// Iterates through the GUIPanels and the lambdaPanels vectors and draws them to the screen
	void DrawGUI() {
		for (unsigned int i = 0; i < m_guiPanels.size(); i++) {
			m_guiPanels[i]->draw(m_activeScene);
		}
		for (unsigned int i = 0; i < m_lambdaPanels.size(); i++) {
			m_lambdaPanels[i]();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Creates a new ImGUI frame
	void CreateNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	// Creates an imgui window by passing in a GUIPanel object
	void AddPanel(std::unique_ptr<GUIPanel> panel) {
		m_guiPanels.push_back(std::move(panel));
	}

	// Creates an imgui window by passing in a lambda function
	void AddPanel(std::function<void()> renderCallback) {
		m_lambdaPanels.push_back(renderCallback);
	}

	void SetScene(Scene* scene) {
		m_activeScene = scene;
	}

	void DrawDemoWindow() {
		ImGui::ShowDemoWindow();
	}
private:
	std::vector<std::unique_ptr<GUIPanel>>	m_guiPanels;
	std::vector<std::function<void()>>		m_lambdaPanels;
};