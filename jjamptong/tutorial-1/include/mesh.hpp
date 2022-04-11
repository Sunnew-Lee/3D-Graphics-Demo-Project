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

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.hpp>


/*  Vertex format */
struct Vertex
{
    Vertex(Vec3 pos, Vec3 nrm, Vec3 uv) : pos(pos), nrm(nrm), uv(uv) {}

    Vertex() { }

    Vec3 pos, nrm;
    Vec2 uv;
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
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) },
    { 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;    // for now numAttribs = 1: only pos

inline float lerp(const float& lo, const float& hi, const float& t)
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
inline float smoothstepDeriv(const float& t)
{
    return t * (6 - 6 * t);
}
inline float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

/*  Mesh format */
class Perlin_Noise
{
public:
    Perlin_Noise(const unsigned& seed = 2016);

    float eval(const glm::vec3& p, glm::vec3& derivs) const;
    float gradientDotV(uint8_t perm, float x, float y, float z) const;
    uint8_t hash(const int& x, const int& y, const int& z) const
    {
        return permutationTable[permutationTable[permutationTable[x] + y] + z];
    }

    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    glm::vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];

};

struct Mesh
{
    Mesh() : numVertices(0), numTris(0), numIndices(0) { }

    /*  Storing the actual vertex/index data */
    std::vector<Vertex> vertexBuffer;
    std::vector<int> indexBuffer;

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

inline double x_pos{ 0.0 };
/*  Mesh function(s) */
Mesh CreatePlane(int stacks, int slices);
Mesh CreateCube(int stacks, int slices);
Mesh CreateSphere(int stacks, int slices);
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle);
Mesh CreateCylinder(int stacks, int slices);
Mesh CreateCone(int stacks, int slices);

Mesh CreateTerrain(int stacks, int slices, double dt, float& frequency);



/******************************************************************************/
/*  Pre-defined shapes                                                        */
/******************************************************************************/
//enum MeshID { PLANE, CUBE, SPHERE, CYLINDER, CONE, PARTIAL_TORUS, TORUS, NUM_MESHES };
enum MeshID { PLANE, NUM_MESHES };

static Mesh mesh[NUM_MESHES] = {   //CreatePlane(50,50),
                                    //CreateCube(1, 1),       /*  For torso, feet */
                                    //CreateSphere(16, 16),   /*  For head */
                                    //CreateCylinder(1, 8),   /*  For arms, legs */
                                    //CreateCone(16, 8),      /*  For hat */
                                    //CreateTorus(4, 8, QUARTER_PI, TWO_PI - QUARTER_PI), /*  For hands */
                                    //CreateTerrain(16, 32)                      /*  For base */
};

#endif