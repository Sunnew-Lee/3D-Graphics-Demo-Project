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

//#include <Noise.h>
#include <PerlinNoise.h>
#include <Perlin_Terrain.h>
#include <CatmullRomSplines.h>

static void draw();
static void update();
static void init();
static void cleanup();

//����

//ValueNoise valueNoise;
PerlinNoise perlinNoise;
Perlin_Terrain perlinTerrain;
CatmullRomSplines catmullRomSplines;

//**********************************************************************************************************//
//Release ----> PerlinNoise
//Debug	  ----> PerlinTerrain
//**********************************************************************************************************//

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


	catmullRomSplines.init();
}

static void update() {
	glfwPollEvents();

	// time between previous and current frame
	double delta_time = GLHelper::update_time(1.0);

	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
	glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());


	catmullRomSplines.update(delta_time);
}

static void draw() {
	ImGui::Render();


	catmullRomSplines.draw();

	glfwSwapBuffers(GLHelper::ptr_window);
}

static void cleanup() {


	catmullRomSplines.cleanup();

	GLHelper::cleanup();
}
