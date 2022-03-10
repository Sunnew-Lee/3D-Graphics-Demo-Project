/*!
@file    main.cpp
@author  pghali@digipen.edu
@date    10/11/2016
@co-author : Dong-A Choi, Sunwoo Lee

// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

This file uses functionality defined in types GLHelper and GLApp to initialize 
an OpenGL context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include <glhelper.h>
#include <glapp.h>
#include <iostream>
#include <sstream>
#include <iomanip>

/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
static void draw();
static void update();
static void init();
static void cleanup();

/*                                                      function definitions
----------------------------------------------------------------------------- */
/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/
int main() {
  init();

  while (!glfwWindowShouldClose(GLHelper::ptr_window)) {

    update();

    draw();
  }

  cleanup();
}

/*  _________________________________________________________________________ */
/*! update
@param none
@return none

Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
For now, there are no objects to animate nor keyboard, mouse button click,
mouse movement, and mouse scroller events to be processed.
*/
static void update() {
  glfwPollEvents();

  // time between previous and current frame
  double delta_time = GLHelper::update_time(1.0);

  std::stringstream sstr;
  sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
  glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

  GLApp::update(delta_time);
}

/*  _________________________________________________________________________ */
/*! draw
@param none
@return none

Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
For now, there's nothing to draw - just paint color buffer with constant color
*/
static void draw() {
  GLApp::draw();
  
  glfwSwapBuffers(GLHelper::ptr_window);
}

/*  _________________________________________________________________________ */
/*! init
@param none
@return none

Get handle to OpenGL context through GLHelper::GLFWwindow*.
*/
static void init() {
  if (!GLHelper::init(1200, 675, "CS250 Project")) {
    std::cout << "Unable to create OpenGL context" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  GLApp::init();
 
}

/*  _________________________________________________________________________ */
/*! cleanup
@param none
@return none

Return allocated resources for window and OpenGL context thro GLFW back
to system.
Return graphics memory claimed through 
*/
void cleanup() {
  GLHelper::cleanup();
  GLApp::cleanup();
}
