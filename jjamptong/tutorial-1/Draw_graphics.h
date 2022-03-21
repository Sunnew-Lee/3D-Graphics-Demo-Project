#pragma once
// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#ifndef DRAW_GRAPHICS_H
#define DRAW_GRAPHICS_H

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "glslshader.h"
#include "graphics.h"
#include "Mesh.h"
#include "../object.h"

struct Draw_graphics {
	static void init();
	static void update(double delta_time);
	static void draw();
	static void cleanup();

	struct GLModel {
		GLenum primitive_type{ 0 };
		GLSLShader shdr_pgm;
		GLuint vaoid{ 0 };
		GLuint idx_elem_cnt{ 0 };

		void setup_vao();
		void setup_shdrpgm();
		void draw();

		GLuint MVpMatLoc, colorLoc;

	};
	static GLModel mdl;

	
};

#endif