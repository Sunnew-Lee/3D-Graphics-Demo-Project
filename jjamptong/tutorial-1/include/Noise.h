// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

#pragma once
#include <vector>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include "glslshader.h"

template<typename T>
class Vec2
{
public:
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}
    Vec2 operator * (const T& r) const { return Vec2(x * r, y * r); }
    Vec2& operator *= (const T& r) { x *= r, y *= r; return *this; }
    T x, y;
};

typedef Vec2<float> Vec2f;

template<typename T = float>
inline T lerp(const T& lo, const T& hi, const T& t)
{
    return lo * (1 - t) + hi * t;
}

inline float smoothstep(const float& t) { return t * t * (3 - 2 * t); }

inline
float quintic(const float& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline
float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

class ValueNoise
{
public:
    ValueNoise(unsigned seed = 2016);
    float eval(Vec2f const& p) const;
    void init();
    void makePPM();
    void makePPM4Marble();
    void makePPM4Wood();
    void mesh_setup();
    GLuint texture_setup();
    void draw();
    void update(double delta_time);
    void cleanUp();

private:
    static const unsigned kMaxTableSize = 256;
    static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
    float r[kMaxTableSize];
    unsigned permutationTable[kMaxTableSize * 2];

    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
        glm::vec2 texcoords;
    };
    GLSLShader shdr;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    unsigned char ptr_texels[256][256 * 3] = { 0 };
    std::vector<GLushort> indices;


    const char* items[3] = { "Value", "Marble", "Wood"};
};