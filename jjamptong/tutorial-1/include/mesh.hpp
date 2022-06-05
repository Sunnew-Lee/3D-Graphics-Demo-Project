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
#include <glslshader.h>
#include <PerlinNoise.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>


/*  Vertex format */
struct Vertex
{
    Vertex(Vec3 pos, Vec3 nrm, Vec2 uv) : pos(pos), nrm(nrm), uv(uv){}

    Vertex() : pos({ 0,0,0 }), nrm({ 0,0,0 }), uv({ 0,0 }) {}

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
    __int64 offset;
};

const VertexLayout vLayout[] =
{
    { 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos) },
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) },
    { 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;    // for now numAttribs = 1: only pos


typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

/*  Mesh format */
struct Mesh
{
    Mesh() : numVertices(0), numTris(0), numIndices(0){ }

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
    GLuint VAO = 0;
    GLuint VBO=0;
    GLuint IBO=0;
    /// /////////////////////////
    GLSLShader shdr_pgm;

    GLSLShader GetShdr_pgm() { return shdr_pgm; };
    //GLint modelLoc;
    //GLint viewLoc;
    //GLint   projectionLoc;
    //GLint   LightLoc;
    //GLint colorLoc;
    //GLint  ViewPosLoc;

    GLint textureLoc{ 0 }, colorLoc{ 0 }, mvpMatLoc{ 0 }, modelLoc{ 0 }, lightPosLoc{ 0 }, sin_valLoc{ 0 }, shrinkLoc{ 0 }, centerLoc{ 0 }, heightLoc{ 0 }, uColorLoc{ 0 }, InnerLoc{ 0 }, OuterLoc{ 0 };
    float shrink = 0.9f;
    float center = 0.75f;
    float grassHeight = 5.0f;
    glm::vec3 uColor = Vec3(0.56, 0.8, 0.56);
    float Inner = 1.f;
    float Outer = 1.f;

    glm::vec3 position{ 0,0,0 };
    glm::vec3 scale{ 1,1,1 };
    glm::vec3 rotation{ 0,0,0 };
    glm::vec4 selfColor{ 1,0,0,1 };

    glm::vec3 lightPos{ 0.0,0.0,-3.0 };

    Mat4 selfMat{ Mat4(1.0f) }, viewMat{ Mat4(1.0f) }, projMat{ Mat4(1.0f) }, MVPMat{ Mat4(1.0f) };
    int stack_{ 0 }, slice_{ 0 };
    double x_pos{ 0.0 };


    void init(std::vector<std::pair<GLenum, std::string>>& shdr_files, glm::vec4 selfcol, glm::vec3 Pos = { 0,0,0 }, glm::vec3 Scale = { 1,1,1 }, glm::vec3 Rotate = { 0,0,0 });
    void SendVertexData();
    void UpdateVertexData();
    void UpdateTerrain(double dt, float& frequency);
    void setup_shdrpgm(std::vector<std::pair<GLenum, std::string>>& shdr_files);
    void setup_mesh();
    void compute_matrix(float delta_time, glm::highp_ivec3 eye, glm::mat4 frustum);
	void draw(/*glm::vec3 color, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos, glm::vec3 view_pos*/);
    void update_tess(int &eye);
    void set_position(glm::vec3 pos)
    {
        position = pos;
    }
    void set_rotation(glm::vec3 angle)
    {
        rotation = angle;
    }
    glm::vec3& Get_position()
    {
        return position;
    }
    glm::vec3& Get_Rotation()
    {
        return rotation;
    }
};


/*  Mesh function(s) */
Mesh CreatePlane(int stacks, int slices);
Mesh* CreateTerrain(int stacks, int slices, float& frequency);
Mesh CreateCube(int stacks, int slices);
Mesh CreateSphere(int stacks, int slices);
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle);
Mesh CreateCylinder(int stacks, int slices);
Mesh CreateCone(int stacks, int slices);


/******************************************************************************/
/*  Pre-defined shapes                                                        */
/******************************************************************************/
//enum MeshID { PLANE, CUBE, SPHERE, CYLINDER, CONE, TORUS, NUM_MESHES };

//static Mesh mesh[NUM_MESHES] =  {   CreatePlane(50,50),
//                                    CreateCube(1, 1),       /*  For torso, feet */
//                                    CreateSphere(16, 16),   /*  For head */
//                                    CreateCylinder(1, 8),   /*  For arms, legs */
//                                    CreateCone(16, 8),      /*  For hat */
//                                    //CreateTorus(4, 8, QUARTER_PI, TWO_PI - QUARTER_PI), /*  For hands */
//                                    CreateTorus(16, 32, 0, TWO_PI)                      /*  For base */
//                                };

#endif