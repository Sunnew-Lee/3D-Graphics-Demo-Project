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

#include "math.hpp"
#include <glhelper.h>

/*  Public functions, so far only allow these interactions */
class Camera
{
public:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveCloser();
	void MoveFarther();

	bool eyeMoved = true;     /*  to trigger view matrix update */
	bool resized = true;     /*  to trigger projection matrix update */

	int eyeRadius = 3;
	glm::mat4 Get_frustum();
	glm::highp_ivec3 Get_eye();
	void Set_eye(int rad);
private:

	/*  Limit for camera angle */
	const int EYE_MAX_ALPHA = NUM_STEPS_PI / 2 - 1;
	const int EYE_MIN_ALPHA = -EYE_MAX_ALPHA + 1;
	const int EYE_ANGLE_STEP = 1;
	/*  Limit for camera distance from the origin */
	const int EYE_MAX_RADIUS = 20;
	const int EYE_MIN_RADIUS = 2;
	const int EYE_RADIUS_STEP = 1;

	/*  Camera view volume planes */
	float nearPlane = 1.0f;
	float farPlane = 80.0f;
	float topPlane = 0.6f * nearPlane;
	float bottomPlane = -topPlane;
	float aspect = 1.0f * GLHelper::width / GLHelper::height;
	float rightPlane = topPlane * aspect;
	float leftPlane = -rightPlane;

	/*  For moving the camera */
	int eyeAlpha = 0;
	int eyeBeta = NUM_STEPS_PI / 2;
	





};

#endif