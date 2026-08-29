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

	void draw(Scene* scene, EditorContext& context) override {
		ImGui::Begin("Inspector", &isOpen, ImGuiWindowFlags_NoMove);

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;

		// Draw Sliders to adjust transform properties
		if (context.selectedObjectID != -1) {
			SceneObject* activeObj = scene->getObjectByID(context.selectedObjectID);
			ImGui::Text("Selected Object: %s", activeObj->name.c_str());
			ImGui::DragFloat3("Position", &activeObj->transform.position.x, 0.5f);
			ImGui::DragFloat3("Scale", &activeObj->transform.scale.x, 0.5f);
		}
		else {
			ImGui::Text("No object selected!");
		}
		
		ImGui::SetWindowPos(glm::vec2(0.0f));
		ImGui::SetWindowSize(ImVec2(screenSize.x * .20f, screenSize.y));
		
		ImGui::Text("Inspector Panel");
		ImGui::Text("Window Size: %f %f", screenSize.x, screenSize.y);

		ImGui::End();
	}
};