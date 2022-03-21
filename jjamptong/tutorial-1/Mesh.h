/******************************************************************************/
/*!
\file   mesh.hpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glhelper.h>
#include <GLFW/glfw3.h>

constexpr float  PI = 3.14159f;
constexpr float  TWO_PI = 2.0f * PI;
constexpr float  HALF_PI = 0.5f * PI;
constexpr float  QUARTER_PI = 0.25f * PI;
constexpr float  EIGHTH_PI = 0.125f * PI;
constexpr float  SIXTEENTH_PI = 0.0625f * PI;
constexpr float  EPSILON = 0.00001f;

constexpr glm::vec3 XAXIS(1.0f, 0.0f, 0.0f);
constexpr glm::vec3 YAXIS(0.0f, 1.0f, 0.0f);

constexpr int    NUM_STEPS_PI = 36;
constexpr int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
constexpr float  ONE_STEP = PI / NUM_STEPS_PI;

static float RoundDecimal(float input)
{
    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}
static glm::vec3 RoundDecimal(const glm::vec3& input)
{
    return glm::vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}
static glm::mat4 Translate(float tx, float ty, float tz)
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
}
static glm::mat4 Translate(const glm::vec3& t)
{
    return glm::translate(glm::mat4(1.0f), t);
}
static glm::mat4 Rotate(float angle, const glm::vec3& axis)
{
    return glm::rotate(glm::mat4(1.0f), angle, axis);
}
static glm::mat4 Scale(float sx, float sy, float sz)
{
    return glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
}
static glm::mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta)
{
    glm::vec4 position{ eyeRadius, 0.f, 0.f, 1.f };
    glm::vec3 up{ 0.f, 1.f, 0.f };

    //Setup rotation matrix to rotate the position around the target
    glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, eyeAlpha, glm::vec3{ 0.f, 0.f, -1.f });
    rotation = rotation * glm::rotate(glm::mat4{ 1 }, eyeBeta, glm::vec3{ 0.f, 1.f, 0.f });
    position = position * rotation;

    //Use cross product to calculate correct up vector
    glm::vec3 left = glm::cross(glm::vec3{ position }, up);
    up = glm::cross(left, glm::vec3{ position });

    return glm::lookAt(glm::vec3{ position }, glm::vec3(0, 0, 0), up);
}
static bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

/*  Vertex format */
struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec3 nrm) : pos(pos), nrm(nrm) {}

    Vertex() { }

    glm::vec3 pos, nrm;

};

const int vertexSize = sizeof(Vertex);
const int indexSize = sizeof(int);


/*  The layouts for specifying the offsets of a vertex
    when it is copied into the graphics pipeline. */
struct VertexLayout
{
    int location;
    int size;
    int type;
    bool normalized;
    int offset;
};

const VertexLayout vLayout[] =
{
    { 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos) },
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;    // for now numAttribs = 1: only pos


typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

/*  Mesh format */
struct Mesh
{
    Mesh() : numVertices(0), numTris(0), numIndices(0) { }

    /*  Storing the actual vertex/index data */
    VertexBufferType vertexBuffer;
    IndexBufferType indexBuffer;

    int numVertices;
    int numTris;
    int numIndices;

    /*  Once the buffer data has been copied into the pipeline, these array object and
        buffer objects will store  its "states".
        Later on if we want to render a mesh, we just need to bind the VAO.
        We don't need to copy the buffer data again.
    */
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
};


/*  Mesh function(s) */
Mesh CreatePlane(int stacks, int slices);
Mesh CreateCube(int stacks, int slices);
Mesh CreateSphere(int stacks, int slices);
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle);
Mesh CreateCylinder(int stacks, int slices);
Mesh CreateCone(int stacks, int slices);



/******************************************************************************/
/*  Pre-defined shapes                                                        */
/******************************************************************************/
enum MeshID { CUBE, SPHERE, CYLINDER, CONE, PARTIAL_TORUS, TORUS, NUM_MESHES };

static Mesh mesh[NUM_MESHES] = { CreateCube(1, 1),       /*  For torso, feet */
                                    CreateSphere(16, 16),   /*  For head */
                                    CreateCylinder(1, 8),   /*  For arms, legs */
                                    CreateCone(16, 8),      /*  For hat */
                                    CreateTorus(4, 8, QUARTER_PI, TWO_PI - QUARTER_PI), /*  For hands */
                                    CreateTorus(16, 32, 0, TWO_PI)                      /*  For base */
};

#endif