/******************************************************************************/
/*!
\file   math.hpp
\par    Purpose: Placeholder for math constants/functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le, YOUR_NAME
\par    Email: sang.le\@digipen.edu, YOUR_EMAIL_ID
\par    DigiPen login: sang.le, YOUR_LOGIN_ID
\date   YOUR_COMPLETION_DATE
*/
/******************************************************************************/

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;


/*  Useful constants */
const float  PI             = 3.14159f;
const float  TWO_PI         = 2.0f * PI;
const float  HALF_PI        = 0.5f * PI;
const float  QUARTER_PI     = 0.25f * PI;
const float  EIGHTH_PI      = 0.125f * PI;
const float  SIXTEENTH_PI   = 0.0625f * PI;

const float  DEG_TO_RAD = PI / 180.0f;
const float  RAD_TO_DEG = 180.0f / PI;

const float  EPSILON = 0.00001f;

const int XINDEX = 0;
const int YINDEX = 1;
const int ZINDEX = 2;

const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

/*  The step for rotation */
const int    NUM_STEPS_PI = 36;
const int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
const float  ONE_STEP = PI / NUM_STEPS_PI;



/******************************************************************************/
/*  Shortcuts to GLM functions                                                */
/******************************************************************************/

/******************************************************************************/
/*!
\fn     static Mat4 Scale(float sx, float sy, float sz)
\brief
        Compute scaling matrix
\param  sx
        Scale factor along x
\param  sy
        Scale factor along y
\param  sz
        Scale factor along z
\return
        The resulting scaling matrix
*/
/******************************************************************************/
inline Mat4 Scale(float sx, float sy, float sz)
{
    return glm::scale(Mat4(1.0f), Vec3(sx, sy, sz));
}


/******************************************************************************/
/*!
\fn     static Mat4 Scale(const Vec3 &s)
\brief
        Compute scaling matrix
\param  s
        3-component vector indicating the scale factors along x, y, z
\return
        The resulting scaling matrix
*/
/******************************************************************************/
inline Mat4 Scale(const Vec3 &s)
{
    return glm::scale(Mat4(1.0f), s);
}


/******************************************************************************/
/*!
\fn     static Mat4 Rotate(float angle, const Vec3 &axis)
\brief
        Compute rotation matrix
\param  angle
        Rotation angle
\param  axis
        Rotation axis
\return
        The resulting rotation matrix
*/
/******************************************************************************/
inline Mat4 Rotate(float angle, const Vec3 &axis)
{
    return glm::rotate(Mat4(1.0f), angle, axis);
}


/******************************************************************************/
/*!
\fn     static Mat4 Rotate(float angle, float ax, float ay, float az)
\brief
        Compute rotation matrix
\param  angle
        Rotation angle
\param  ax
        x-component of rotation axis
\param  ay
        y-component of rotation axis
\param  az
        z-component of rotation axis
\return
        The resulting rotation matrix
*/
/******************************************************************************/
inline Mat4 Rotate(float angle, float ax, float ay, float az)
{
    return glm::rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
}


/******************************************************************************/
/*!
\fn     static Mat4 Translate(float tx, float ty, float tz)
\brief
        Compute translation matrix
\param  tx
        Translation along x
\param  ty
        Translation along y
\param  tz
        Translation along z
\return
        The resulting translation matrix
*/
/******************************************************************************/
inline Mat4 Translate(float tx, float ty, float tz)
{
    return glm::translate(Mat4(1.0f), Vec3(tx, ty, tz));
}


/******************************************************************************/
/*!
\fn     static Mat4 Translate(const Vec3 &t)
\brief
        Compute translation matrix
\param  s
        3-component vector indicating the translation amounts along x, y, z
\return
        The resulting translation matrix
*/
/******************************************************************************/
inline Mat4 Translate(const Vec3 &t)
{
    return glm::translate(Mat4(1.0f), t);
}


/******************************************************************************/
/*!
\fn     static Mat4 Frustum(float leftPlane, float rightPlane, 
                            float bottomPlane, float topPlane, 
                            float nearPlane, float farPlane)
\brief
        Compute perspective projection matrix
\param  leftPlane
        x-coordinate of the intersection between left plane and near plane
\param  rightPlane
        x-coordinate of the intersection between right plane and near plane
\param  bottomPlane
        y-coordinate of the intersection between bottom plane and near plane
\param  topPlane
        y-coordinate of the intersection between top plane and near plane
\param  nearPlane
        z-coordinate of near plane
\param  farPlane
        z-coordinate of far plane
\return
        The resulting perspective projection matrix
*/
/******************************************************************************/
inline Mat4 Frustum(float leftPlane, float rightPlane,
                    float bottomPlane, float topPlane, 
                    float nearPlane, float farPlane)
{
    return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}


/******************************************************************************/
/*!
\fn     static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
\brief
        Compute view transformation matrix
\param  eye
        Camera position
\param  center
        Look-at position
\param  up
        Up vector
\return
        The resulting view transformation matrix
*/
/******************************************************************************/
inline Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    return glm::lookAt(eye, center, up);
}


/******************************************************************************/
/*!
\fn     static Mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta)
\brief
        Compute view transformation matrix for a camera that looks at the origin
\param  eyeRadius
        Fixed distance from camera to origin
\param  eyeAlpha
        Angle between origin-camera vector and xz-plane
\param  eyeBeta
        Angle between a vector and the x-axis. The vector is the displacement from
        the origin to point obtained by orthogonal projection of camera position
        onto xz-plane.
\return
        The resulting view transformation matrix
*/
/******************************************************************************/
inline Mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta)
{
    glm::vec4 position{ -eyeRadius, 0.f, 0.f, 1.f };
    glm::vec3 up{ 0.f, 1.f, 0.f };

    //Setup rotation matrix to rotate the position around the target
    glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, eyeAlpha, glm::vec3{ 0.f, 0.f, -1.f });
    rotation = rotation * glm::rotate(glm::mat4{ 1 }, eyeBeta, glm::vec3{ 0.f, 1.f, 0.f });
    position = position * rotation;

    //Use cross product to calculate correct up vector
    glm::vec3 left = glm::cross(glm::vec3{ position }, up);
    up = glm::cross(left, glm::vec3{ position });

    return LookAt(glm::vec3{ position }, Vec3(0, 0, 0), up);
}


/******************************************************************************/
/*!
\fn     template <typename T>static T Normalize(T vec)
\brief
        Normalize a vector
\param  vec
        The input vector
\return
        The resulting normalized vector
*/
/******************************************************************************/
template <typename T>
inline T Normalize(T vec)
{
    return glm::normalize(vec);
}


/******************************************************************************/
/*!
\fn     template <typename T>static float *ValuePtr(T value)
\brief
        Get a pointer to value, normally used for vector/matrix data
\param  value
        The given value
\return
        The pointer to the given value
*/
/******************************************************************************/
template <typename T>
inline float const* ValuePtr(T const& value)
{
    return glm::value_ptr(value);
}


/******************************************************************************/
/*!
\fn     static float RoundDecimal(float input)
\brief
        Round a value to 4-decimal places
\param  input
        The given value
\return
        The rounded value
*/
/******************************************************************************/
inline float RoundDecimal(float input)
{
    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}


/******************************************************************************/
/*!
\fn     static Vec3 RoundDecimal(const Vec3 &input)
\brief
        Round the components of a vector3 to 4-decimal places
\param  input
        The given vector
\return
        The vector with rounded values
*/
/******************************************************************************/
inline Vec3 RoundDecimal(const Vec3 &input)
{
    return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}


/******************************************************************************/
/*!
\fn     static Vec4 RoundDecimal(const Vec4 &input)
\brief
        Round the components of a vector4 to 4-decimal places
\param  input
        The given vector
\return
        The vector with rounded values
*/
/******************************************************************************/
inline Vec4 RoundDecimal(const Vec4 &input)
{
    return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
}


/******************************************************************************/
/*!
\fn     static bool DegenerateTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
\brief
        Check if a triangle is degenerate, that is if the length between any
        two vertices is 0.
\param  v0
        The 1st vertex
\param  v1
        The 2nd vertex
\param  v2
        The 3rd vertex
\return
        Whether the triangle is degenerate
*/
/******************************************************************************/
inline bool DegenerateTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
            glm::distance(v1, v2) < EPSILON ||
            glm::distance(v2, v0) < EPSILON);
}

template<typename T = float>
inline T lerp(const T& lo, const T& hi, const T& t)
{
    return lo * (1 - t) + hi * t;
}

inline float smoothstep(const float& t)
{
    return t * t * (3 - 2 * t);
}

inline float quintic(const float& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

#endif