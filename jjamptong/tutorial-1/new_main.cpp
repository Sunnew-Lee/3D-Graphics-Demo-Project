/******************************************************************************/
/*!
@file   main.cpp
@par    Purpose: Main file of the program
@par    Language: C++
@par    Platform: Visual Studio 2013, Windows 7 64-bit
@author Sang Le
@par    Email: sang.le\@digipen.edu
@par    DigiPen login: sang.le
@date   28/10/2014
*/
/******************************************************************************/

#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include "input.h"
#include "graphics.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


/******************************************************************************/
/*!
@fn     void CreateGLContext(int argc, char **argv)
@brief
        Create the context for OpenGL commands to work.
@param  argc
        argc from main function
@param  argv
        argv from main function
*/
/******************************************************************************/
void CreateGLContext(int argc, char** argv)
{
    /* GLUT Window Creation & Initialization */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LegoLand");

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);


    typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(1);
}


/******************************************************************************/
/*!
@fn     void InitGLExtensions()
@brief
        Initialize the OpenGL extensions supported on the current platform.
*/
/******************************************************************************/
void InitGLExtensions()
{
    /* GL3W Initialization */
    if (glewInit())
    {
        std::cerr << "Failed to initialize OpenGL.\n";
        exit(1);
    }

    if (!glewIsSupported("GL_VERSION_3_3"))
    {
        std::cerr << "OpenGL 3.3 not supported.\n";
        exit(1);
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL version  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/******************************************************************************/
/*!
@fn     void MainLoop()
@brief
        The main loop of OpenGL rendering and user interactions.
*/
/******************************************************************************/
void MainLoop()
{
    /* GLUT callback loop */
    glutDisplayFunc(Render);
    glutIdleFunc(Render);

    glutReshapeFunc(Resize);

    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);

    glutMouseWheelFunc(MouseWheel);

    glutMainLoop();
}


/******************************************************************************/
/*!
@fn     main
@brief
        Entry point of the program.
@param  argc
        Number of arguments when the program is executed.
@param  argv
        The array of arguments when the program is executed.
*/
/******************************************************************************/
int main(int argc, char** argv)
{
    /*  This function can be used in Windows to disable the console.
        Currently, we still keep our console for debugging purpose. */
        //FreeConsole();

    CreateGLContext(argc, argv);
    InitGLExtensions();

    SetUp();

    /*  The main loop for user drawing and interactions */
    MainLoop();
}