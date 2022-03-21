/******************************************************************************/
/*!
\file   input.cpp
\par    Purpose: Handle events upon user inputs
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#include "input.h"

/******************************************************************************/
/*!
\fn     void MoveUp()
\brief
        Handle event when user press w/W
*/
/******************************************************************************/
void MoveUp()
{
    /*  Move camera up */
    if (eyeAlpha < EYE_MAX_ALPHA)
    {
        eyeAlpha += EYE_ANGLE_STEP;
        eyeMoved = true;
        glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveDown()
\brief
        Handle event when user press s/S
*/
/******************************************************************************/
void MoveDown()
{
    /*  Move camera down */
    if (eyeAlpha > EYE_MIN_ALPHA)
    {
        eyeAlpha -= EYE_ANGLE_STEP;
        eyeMoved = true;
        glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveLeft()
\brief
        Handle event when user press a/A
*/
/******************************************************************************/
void MoveLeft()
{
    /*  Move camera left */
    eyeBeta += EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta > TWO_NUM_STEPS_PI)
        eyeBeta -= TWO_NUM_STEPS_PI;

    eyeMoved = true;
    glutPostRedisplay();    /*  Set flag to force re-rendering */
}


/******************************************************************************/
/*!
\fn     void MoveRight()
\brief
        Handle event when user press d/D
*/
/******************************************************************************/
void MoveRight()
{
    /*  Move camera right */
    eyeBeta -= EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta < 0)
        eyeBeta += TWO_NUM_STEPS_PI;

    eyeMoved = true;
    glutPostRedisplay();    /*  Set flag to force re-rendering */
}


/******************************************************************************/
/*!
\fn     void MoveCloser()
\brief
        Handle event when user press UP or scroll mouse wheel down
*/
/******************************************************************************/
void MoveCloser()
{
    /*  Move camera closer to origin */
    if (eyeRadius > EYE_MIN_RADIUS)
    {
        eyeRadius -= EYE_RADIUS_STEP;
        eyeMoved = true;
        glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveFarther()
\brief
        Handle event when user press DOWN or scroll mouse wheel up
*/
/******************************************************************************/
void MoveFarther()
{
    /*  Move camera farther from origin */
    if (eyeRadius < EYE_MAX_RADIUS)
    {
        eyeRadius += EYE_RADIUS_STEP;
        eyeMoved = true;
        glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void Keyboard(unsigned char key, int x, int y)
\brief
        Events when a normal key is pressed.
\param  key
        The pressed key.
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 9:     /*  TAB */
        switch (currRenderMode)
        {
        case WIREFRAME:
            /*  Switch to drawing using color/texture */
            currRenderMode = COLOR;
            /*  Draw filled triangles and enable back-face culling */
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
            break;

        case COLOR:
            /*  Switch to drawing using normals */
            currRenderMode = NORMAL;
            /*  Draw filled triangles and enable back-face culling */
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
            break;

        case NORMAL:
            /*  Switch to wireframe mode */
            currRenderMode = WIREFRAME;
            /*  Draw lines and disable back-face culling */
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            break;
        }
        glutPostRedisplay();    /*  Set flag to force re-rendering */
        break;

    case 27:    /*  ESC */
        CleanUp();
        break;

    case ' ':
        animated = !animated;
        if (animated)
            justAnimated = true;

        glutPostRedisplay();    /*  Set flag to force re-rendering */
        break;

    case 'w':
    case 'W':
        MoveUp();
        break;

    case 's':
    case 'S':
        MoveDown();
        break;

    case 'a':
    case 'A':
        MoveLeft();
        break;

    case 'd':
    case 'D':
        MoveRight();
        break;
    }
}


/******************************************************************************/
/*!
\fn     void SpecialKeyboard(int key, int x, int y)
\brief
        Events when a special key is pressed. This include CTRL+ ..., ALT+ ...
\param  key
        The pressed key(s).
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void SpecialKeyboard(int key, int x, int y)
{
    /* ALT+F4: Quit */
    if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT)
        CleanUp();
    else
        switch (key)
        {
        case GLUT_KEY_UP:
            MoveCloser();
            break;

        case GLUT_KEY_DOWN:
            MoveFarther();
            break;

            /*  Not using LEFT/RIGHT for now */
            //case GLUT_KEY_LEFT: 
            //    break;

            //case GLUT_KEY_RIGHT: 
            //    break;
        }
}


/******************************************************************************/
/*!
\fn     void MouseWheel(int wheel, int direction, int x, int y)
\brief
        Events when the mouse wheel is scrolled.
\param  wheel
        The scrolled wheel.
\param  direction
        The scrolling direction.
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MouseWheel(int wheel, int direction, int x, int y)
{
    switch (direction)
    {
    case 1:
        MoveFarther();
        break;
    case -1:
        MoveCloser();
        break;
    }
}