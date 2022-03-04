/*!
@file    main.cpp
@author  pghali@digipen.edu
@date    10/11/2016

This file uses functionality defined in type GLApp to initialize an OpenGL
context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include "../include/glapp.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glm/glm.hpp>

/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
static void draw();
static void update();
static void init();
static void cleanup();

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
GLApp gl_app;

/*                                                      function definitions
----------------------------------------------------------------------------- */
/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/
int main() {
	init();
	while (!glfwWindowShouldClose(GLApp::ptr_window)) {
		update();
		draw();
	}
	cleanup();
}

/*  _________________________________________________________________________ */
/*! init
@param none
@return none

Get handle to OpenGL context through GLHelper::GLFWwindow*.
*/
static void init() {
	const glm::ivec2 window_size{ 1980, 1080 };
	if (!gl_app.init(window_size.x, window_size.y, "Dong-A Choi")) {
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(GLApp::ptr_window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

/*! update

@param none
Handle to window that defines the OpenGL context

@return none

For now, there are no objects to animate nor keyboard, mouse button click,
mouse movement, and mouse scroller events to be processed.
The only event triggered is window resize.
*/
static void update() {
	// time between previous and current frame
	double delta_time = gl_app.update_time(1.0);
	// write window title with current fps ...
	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(2) << GLApp::title << ": " << GLApp::fps;
	glfwSetWindowTitle(GLApp::ptr_window, sstr.str().c_str());

	glfwPollEvents();

	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Display FPS in another viewport
	ImGui::Begin("Scene");
	ImGui::Text("Application average %0.3f ms/frame (%.01f FPS)", (1.f / GLApp::fps) * 1000.f, GLApp::fps);
	ImGui::End();
}

/*  _________________________________________________________________________ */
/*! draw

@param none
Handle to window that defines the OpenGL context

@return none

For now, there's nothing to draw - just paint colorbuffer with constant color
*/
static void draw() {
	// clear colorbuffer with RGBA value in glClearColor ...
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f, 1.f, 0.f, 1.f);

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// swap buffers: front <-> back
	glfwSwapBuffers(GLApp::ptr_window);
}

/*  _________________________________________________________________________ */

/*! cleanup
@param none
@return none

Return allocated resources for window and OpenGL context thro GLFW back
to system.
Return graphics memory claimed through
*/
void cleanup() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	GLApp::cleanup();
}
