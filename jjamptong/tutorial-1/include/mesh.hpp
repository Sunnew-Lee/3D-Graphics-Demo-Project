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
    Vertex(Vec3 pos, Vec3 nrm, Vec3 uv) : pos(pos), nrm(nrm), uv(uv){}

    Vertex(){ }

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


/*  Mesh format */
struct Mesh
{
    Mesh() : numVertices(0), numTris(0), numIndices(0){ }

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
//enum MeshID { PLANE, CUBE, SPHERE, CYLINDER, CONE, PARTIAL_TORUS, TORUS, NUM_MESHES };
enum MeshID { PLANE, NUM_MESHES };

static Mesh mesh[NUM_MESHES] =  {   CreatePlane(7,7),
                                    //CreateCube(1, 1),       /*  For torso, feet */
                                    //CreateSphere(16, 16),   /*  For head */
                                    //CreateCylinder(1, 8),   /*  For arms, legs */
                                    //CreateCone(16, 8),      /*  For hat */
                                    //CreateTorus(4, 8, QUARTER_PI, TWO_PI - QUARTER_PI), /*  For hands */
                                    //CreatePlane(16, 32)                      /*  For base */
                                };

#endif