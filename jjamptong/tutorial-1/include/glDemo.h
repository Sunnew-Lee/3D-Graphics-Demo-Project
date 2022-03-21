#pragma once

#ifndef GLDEMO_H
#define GLDEMO_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glhelper.h>


struct GLDemo {
	static void init();
	static void update(double delta_time);
	static void draw();
	static void cleanup();

};

#endif

