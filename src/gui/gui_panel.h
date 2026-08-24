#pragma once

#include "imgui/imgui.h"

class GUIPanel {
public:
	virtual ~GUIPanel() = default;

	virtual void draw() = 0;

	bool isOpen = true;
};