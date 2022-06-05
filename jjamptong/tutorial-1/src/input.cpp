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

#include "input.hpp"

/******************************************************************************/
/*!
\fn     void MoveUp()
\brief
        Handle event when user press w/W
*/
/******************************************************************************/
void Camera::MoveUp()
{
    /*  Move camera up */
    if (eyeAlpha < EYE_MAX_ALPHA)
    {
        eyeAlpha += EYE_ANGLE_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveDown()
\brief
        Handle event when user press s/S
*/
/******************************************************************************/
void Camera::MoveDown()
{
    /*  Move camera down */
    if (eyeAlpha > EYE_MIN_ALPHA)
    {
        eyeAlpha -= EYE_ANGLE_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveLeft()
\brief
        Handle event when user press a/A
*/
/******************************************************************************/
void Camera::MoveLeft()
{
    /*  Move camera left */
    eyeBeta += EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta > TWO_NUM_STEPS_PI)
        eyeBeta -= TWO_NUM_STEPS_PI;

    eyeMoved = true;
    //glutPostRedisplay();    /*  Set flag to force re-rendering */
}


/******************************************************************************/
/*!
\fn     void MoveRight()
\brief
        Handle event when user press d/D
*/
/******************************************************************************/
void Camera::MoveRight()
{
    /*  Move camera right */
    eyeBeta -= EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta < 0)
        eyeBeta += TWO_NUM_STEPS_PI;

    eyeMoved = true;
    //glutPostRedisplay();    /*  Set flag to force re-rendering */
}


/******************************************************************************/
/*!
\fn     void MoveCloser()
\brief
        Handle event when user press UP or scroll mouse wheel down
*/
/******************************************************************************/
void Camera::MoveCloser()
{
    /*  Move camera closer to origin */
    if (eyeRadius > EYE_MIN_RADIUS)
    {
        eyeRadius -= EYE_RADIUS_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}


/******************************************************************************/
/*!
\fn     void MoveFarther()
\brief
        Handle event when user press DOWN or scroll mouse wheel up
*/
/******************************************************************************/
void Camera::MoveFarther()
{
    /*  Move camera farther from origin */
    if (eyeRadius < EYE_MAX_RADIUS)
    {
        eyeRadius += EYE_RADIUS_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}

glm::mat4 Camera::Get_frustum()
{
    return Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}

glm::highp_ivec3 Camera::Get_eye()
{
    return glm::highp_ivec3(eyeAlpha, eyeBeta, eyeRadius);
}

void Camera::Set_eye(int rad)
{
    eyeRadius = rad;
}