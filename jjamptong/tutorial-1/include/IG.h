#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

struct IG
{
	static void init();
	static void update();
	static void draw();
	static void cleanup();
};