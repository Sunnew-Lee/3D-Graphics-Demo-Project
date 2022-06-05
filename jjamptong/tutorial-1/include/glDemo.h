// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once

#ifndef GLDEMO_H
#define GLDEMO_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glhelper.h>
#include <imgui.h>


class GLDemo
{
public:
	GLDemo() {};
	virtual ~GLDemo() {};

	virtual void init()=0;
	virtual void update(double delta_time)=0;
	virtual void draw()=0;
	virtual void cleanup()=0;


private:

};

#endif

