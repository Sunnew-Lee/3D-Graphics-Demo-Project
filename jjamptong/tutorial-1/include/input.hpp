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

#ifndef INPUT_HPP
#define INPUT_HPP

#include "graphics.hpp"

/*  Public functions, so far only allow these interactions */
void MoveUp();
void MoveDown();
void MoveLeft();
void MoveRight();
void MoveCloser();
void MoveFarther();

#endif

