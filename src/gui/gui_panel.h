#pragma once

#include "imgui/imgui.h"

class GUIPanel {
public:
	virtual ~GUIPanel() = default;

	virtual void draw(Scene* scene) = 0;

	bool isOpen = true;
};