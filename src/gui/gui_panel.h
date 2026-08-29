#pragma once

#include "imgui/imgui.h"
#include "editor_context.h"

class Scene;

class GUIPanel {
public:
	virtual ~GUIPanel() = default;

	virtual void draw(Scene* scene, EditorContext& context) = 0;

	bool isOpen = true;
};