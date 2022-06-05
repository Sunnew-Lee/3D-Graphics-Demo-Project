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
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <IG.h>
#include <glDemo.h>

#include <Model_demo.h>
#include <Toon_Shading-Fog.h>
#include <Noise.h>
#include <PerlinNoise.h>
#include <Perlin_Terrain.h>
#include <Hermite_Curve.h>
#include <CatmullRomSplines.h>
#include <vector>

/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
//static void draw();
//static void update();
//static void init();
//static void cleanup();

//¿¹½Ã


int main() {
    if (!GLHelper::init(1200, 675, "CS250 Project")) {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    IG* main_IG = new IG();

    std::vector<GLDemo*> demos;
    Models* model = new Models();
    Toon_Fog* toonfog = new Toon_Fog();
    ValueNoise* valnoise = new ValueNoise();
    PerlinNoise* pernoise = new PerlinNoise();
    Perlin_Terrain* terrain = new Perlin_Terrain();
    Hermite_Curve* hercurve = new Hermite_Curve();
    CatmullRomSplines* catcurve = new CatmullRomSplines();

    demos.push_back(model);
    demos.push_back(toonfog);
    demos.push_back(valnoise);
    demos.push_back(pernoise);
    demos.push_back(terrain);
    demos.push_back(hercurve);
    demos.push_back(catcurve);
    
    IG::DEMOS curr_demo = main_IG->Get_state();

    if (curr_demo != IG::DEMOS::NONE)
    {
        demos[static_cast<int>(curr_demo)]->init();
    }

    main_IG->init();
    //IG::init();

  while (!glfwWindowShouldClose(GLHelper::ptr_window)) {

      glfwPollEvents();

      // time between previous and current frame
      double delta_time = GLHelper::update_time(1.0);

      std::stringstream sstr;
      sstr << std::fixed << std::setprecision(2) << GLHelper::title << ": " << GLHelper::fps;
      glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());

      main_IG->update();
      
      if (curr_demo != main_IG->Get_state())
      {
          demos[static_cast<int>(curr_demo)]->cleanup();

          curr_demo = main_IG->Get_state();
          demos[static_cast<int>(curr_demo)]->init();
          continue;
      }

     
      if (curr_demo != IG::DEMOS::NONE)
      {
          demos[static_cast<int>(curr_demo)]->update(delta_time);
      }
      
      //IG::update();
      //===================================================================================================
  	  ImGui::Render();

      //curr_demo = main_IG->Get_state();
      if (curr_demo != IG::DEMOS::NONE)
      {
          ImVec4 clear_col = main_IG->get_clearcolor();
          glClearColor(clear_col.x * clear_col.w, clear_col.y * clear_col.w, clear_col.z * clear_col.w, clear_col.w);
          
          demos[static_cast<int>(curr_demo)]->draw();
      }
      //glClear(GL_COLOR_BUFFER_BIT);
      //IG::draw();
      main_IG->draw();

      glfwSwapBuffers(GLHelper::ptr_window);
  }

  ////////////////////////////////////////////////////////////////////
  //curr_demo = main_IG->Get_state();
  if (curr_demo != IG::DEMOS::NONE)
  {
      demos[static_cast<int>(curr_demo)]->cleanup();
  }
  main_IG->cleanup();
  //IG::cleanup();
  GLHelper::cleanup();
}

//static void init() {
//
//}
//
//static void update() {
//
//}
//
//static void draw() {
//
//}
//
//static void cleanup() {
//
//}
