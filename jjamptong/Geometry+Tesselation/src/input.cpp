// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include "input.hpp"
#include <graphics.hpp>

void MoveUp()
{
    /*  Move camera up */
    if (eyeAlpha < EYE_MAX_ALPHA)
    {
        eyeAlpha += EYE_ANGLE_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}

void MoveDown()
{
    /*  Move camera down */
    if (eyeAlpha > EYE_MIN_ALPHA)
    {
        eyeAlpha -= EYE_ANGLE_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}

void MoveLeft()
{
    /*  Move camera left */
    eyeBeta += EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta > TWO_NUM_STEPS_PI)
        eyeBeta -= TWO_NUM_STEPS_PI;

    eyeMoved = true;
    //glutPostRedisplay();    /*  Set flag to force re-rendering */
}

void MoveRight()
{
    /*  Move camera right */
    eyeBeta -= EYE_ANGLE_STEP;

    /*  Keep eyeBeta within [0, 360] */
    if (eyeBeta < 0)
        eyeBeta += TWO_NUM_STEPS_PI;

    eyeMoved = true;
    //glutPostRedisplay();    /*  Set flag to force re-rendering */
}

void MoveCloser()
{
    /*  Move camera closer to origin */
    if (eyeRadius > EYE_MIN_RADIUS)
    {
        eyeRadius -= EYE_RADIUS_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}

void MoveFarther()
{
    /*  Move camera farther from origin */
    if (eyeRadius < EYE_MAX_RADIUS)
    {
        eyeRadius += EYE_RADIUS_STEP;
        eyeMoved = true;
        //glutPostRedisplay();    /*  Set flag to force re-rendering */
    }
}