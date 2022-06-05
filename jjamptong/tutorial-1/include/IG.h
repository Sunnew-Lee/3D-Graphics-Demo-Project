// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

class IG
{
public:
	enum class DEMOS
	{
		MODEL = 0,
		TOON,
		VALUE,
		PERLIN,
		TERRAIN,
		HERMITE,
		CATMULL,
		//GEOM,
		//SHADOW,
		NONE
	};

	IG() :curr_demo(DEMOS::MODEL), selectedItem(0), is_wireframe(false)  {};
	void init();
	void update();
	void draw();
	void cleanup();
	void Set_state(DEMOS next_demo);
	DEMOS Get_state();
	ImVec4& get_clearcolor()
	{
		return clear_color;
	};

private:
	const char* items[static_cast<int>(DEMOS::NONE)]
	{
		"Procedural Geometric Modeling",
		"Toon / Cell Shading & Fog",
		"Value noise",
		"Gradient noise / Perlin noise",
		"Perlin Terrain",
		"Hermite Curve",
		"Catmull-Rom Spline",
		//"Geometry & Tessellation Shaders",
		//"Shadow Mapping"
	};
	int selectedItem;
	DEMOS curr_demo;
	bool is_wireframe;
	ImVec4 clear_color{ ImVec4(0.0f, 0.0f, 0.0f, 1.0f) };
};