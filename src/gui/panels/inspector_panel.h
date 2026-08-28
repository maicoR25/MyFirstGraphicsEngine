#pragma once

#include "../gui_panel.h"

class Scene;
class SceneObject;

class InspectorPanel : public GUIPanel{
public:
	SceneObject* currentObject = nullptr;

	void setSelected(SceneObject* object) {
		currentObject = object;
	}

	void draw(Scene* scene) override {
		ImGui::Begin("Inspector", &isOpen, ImGuiWindowFlags_NoMove);

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;

		// Draw Sliders to adjust transform properties
		if (hasSelection) {
			SceneObject* activeObj = scene->getObjectByID(0);
			ImGui::DragFloat3("Position", &activeObj->transform.position.x, 0.1f);
		}
		else {
			ImGui::Text("No object selected!");
		}
		
		ImGui::SetWindowPos(glm::vec2(0.0f));
		ImGui::SetWindowSize(ImVec2(screenSize.x * .20f, screenSize.y));
		
		ImGui::Text("Inspector Panel");
		ImGui::Text("Window Size: %f %f", screenSize.x, screenSize.y);

		ImGui::Text("FPS: %f");
		ImGui::End();
	}
private:
	bool hasSelection = true;
	unsigned int selectedObjectID = 0;
};