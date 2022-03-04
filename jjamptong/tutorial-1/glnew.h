// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once

#ifndef GLNEW_H
#define GLNEW_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glapp.h>

struct GLNew {
	GLNew();
	static void init();
	static void update();
	static void draw();
	static void cleanup();


	static const char* vertexShaderSource;
	static const char* fragmentShaderSource;

	static unsigned int shaderProgram;

	static GLFWwindow* window;
	static unsigned int VBO, VAO;
};

#endif
