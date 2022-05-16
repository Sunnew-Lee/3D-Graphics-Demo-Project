/*!
@file    main.cpp
@author  pghali@digipen.edu
@date    10/11/2016
@co-author : Dong-A Choi, Sunwoo Lee

// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

This file uses functionality defined in types GLHelper and GLApp to initialize
an OpenGL context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include <glhelper.h>
#include <glapp.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <IG.h>

#include <Procedural_Modeling.h>
#include <Toon_Shading-Fog.h>
#include <Tessellation.h>

static void draw();
static void update();
static void init();
static void cleanup();

//����

P_Modeling P_Model;
Toon_Fog toon_fog;
//Geometry geometry;
Tessellation tessellation;


int main() {
	init();

	while (!glfwWindowShouldClose(GLHelper::ptr_window)) {

		update();

		draw();
	}

	cleanup();
}

static void init() {
	if (!GLHelper::init(1200, 675, "CS250 Project")) {
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//GLApp::init();
	//P_Model.init();
	//toon_fog.init();
	tessellation.init();
}

static void update() {
	glfwPollEvents();

	// time between previous and current frame
	double delta_time = GLHelper::update_time(1.0);

	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
	glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

	//GLApp::update(delta_time);
	//P_Model.update(delta_time);
	//toon_fog.update(delta_time);
	tessellation.update(delta_time);
}

static void draw() {
	ImGui::Render();

	//GLApp::draw();
	//P_Model.draw();
	//toon_fog.draw();
	tessellation.draw();

	glfwSwapBuffers(GLHelper::ptr_window);
}

static void cleanup() {
	//GLApp::cleanup();
	//P_Model.cleanup();
	//toon_fog.cleanup();
	tessellation.cleanup();
	GLHelper::cleanup();
}
