/******************************************************************************/
/*!
\file   graphics.hpp
\par    Purpose: Declaring the graphics-related variables & functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "object.h"

#pragma comment (lib, "opengl32.lib")

/*  Viewport width & height, need to access from input.cpp */
extern int width;
extern int height;

/*  For toggling between different rendering modes, need access from input.cpp */
enum RenderMode { COLOR, NORMAL, WIREFRAME };
extern RenderMode currRenderMode;

/*  For toggling animation, need to access from input.cpp  */
extern bool animated, justAnimated;


/*  Public functions */
void SetUp();
void CleanUp();
void Resize(const int w, const int h);
void Render();

#endif