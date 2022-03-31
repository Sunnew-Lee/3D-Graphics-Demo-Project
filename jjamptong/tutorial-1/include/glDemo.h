#pragma once
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glhelper.h>

class GLDemo
{
public:
	GLDemo() {};
	virtual ~GLDemo() {};

	virtual void init() {};
	virtual void update(double delta_time) {};
	virtual void draw() {};
	virtual void cleanup() {};
private:
};

//
//struct GLDemo {
//	static void init();
//	static void update(double delta_time);
//	static void draw();
//	static void cleanup();
//
//};


