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

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include <GL/glew.h> 
#include <GLFW/glfw3.h>
//#include "gl/freeglut.h"

#include "mesh.hpp"
#include "object.hpp"


#pragma comment (lib, "opengl32.lib")
//#pragma comment (lib, "freeglut.lib")


/*  Viewport width & height, need to access from input.cpp */
extern int width;
extern int height;



/*  For moving camera about the origin.
    I'm using int values to avoid imprecision when updating camera position over
    a long period of time.
    These camera values will be multiplied with a predefined portion of PI later to
    compute the final position.
*/
extern int eyeAlpha, eyeBeta;
extern int eyeRadius;
extern bool eyeMoved;
/*  Limit for camera angle */
const int EYE_MAX_ALPHA     = NUM_STEPS_PI / 2 - 1;
const int EYE_MIN_ALPHA     = -EYE_MAX_ALPHA + 1;
const int EYE_ANGLE_STEP    = 1;
/*  Limit for camera distance from the origin */
const int EYE_MAX_RADIUS    = 30;
const int EYE_MIN_RADIUS    = 1;
const int EYE_RADIUS_STEP   = 1;


/*  Public functions */
void SetUp();
void CleanUp();
void Resize(const int w, const int h);
void Render();

#endif