/******************************************************************************/
/*!
\file   input.hpp
\par    Purpose: Handle events upon user inputs
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"

/*  Public functions, so far only allow these interactions */
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void MouseWheel(int wheel, int direction, int x, int y);

#endif

