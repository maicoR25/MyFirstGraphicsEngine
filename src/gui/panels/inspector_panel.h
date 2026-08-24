#pragma once
#include "../gui_panel.h"

class InspectorPanel : public GUIPanel{
public:

	void draw() override {
		ImGui::Begin("Inspector", &isOpen, ImGuiWindowFlags_NoMove);

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		
		ImGui::SetWindowPos(glm::vec2(0.0f));
		ImGui::SetWindowSize(ImVec2(screenSize.x * .20f, screenSize.y));
		
		ImGui::Text("Insepctor Panel");
		ImGui::Text("Window Size: %f %f", screenSize.x, screenSize.y);
		ImGui::End();
	}
};