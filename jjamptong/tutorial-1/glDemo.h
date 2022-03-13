#pragma once

#ifndef GLDEMO_H
#define GLDEMO_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glhelper.h>

#include "glslshader.h"

struct GLDemo {
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
	};
	static GLModel mdl;
};

#endif /* GLAPP_H */
