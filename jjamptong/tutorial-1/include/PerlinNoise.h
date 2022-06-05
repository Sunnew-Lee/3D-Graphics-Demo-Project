// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glslshader.h>
#include <glDemo.h>



class PolyMesh
{
public:
    PolyMesh() : 
        vertices(nullptr), st(nullptr), normals(nullptr), faceArray(nullptr), verticesArray(nullptr){}
    ~PolyMesh()
    {
        if (vertices) delete[] vertices;
        if (st) delete[] st;
        if (normals) delete[] normals;
    }
    glm::vec3* vertices;
    glm::vec2* st;
    glm::vec3* normals;
    uint32_t* faceArray;
    uint32_t* verticesArray;
    uint32_t numVertices = 0;
    uint32_t numFaces = 0;
};

class PerlinNoise : public GLDemo
{
public:
    PerlinNoise(const unsigned& seed = 2016);
    ~PerlinNoise() override {};

    void init() override;
    void update(double delta_time) override;
    void draw() override;
    void cleanup() override;

    void vert_setup();
    void shdr_setup();
    void makePPM();
    void makePPM4Quintic();
    uint8_t hash(const int& x, const int& y, const int& z) const;
    float eval4SmoothStep(const glm::vec3& p) const;
    float gradientDotV(uint8_t perm, float x, float y, float z) const;
    float eval4Quintic(const glm::vec3& p, glm::vec3& derivs) const;
    void mesh_setup();
    GLuint texture_setup();

private:
    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    glm::vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];

    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
        glm::vec2 texcoords;
    };
    GLSLShader shdr;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    unsigned char ptr_texels[256][256*3] = { 0 };
	std::vector<GLushort> indices;



    enum class PERLIN_MODE
    {
        SMOOTH = 0,
        QUINTIC,
        NONE
    };

    const char* items[static_cast<int>(PERLIN_MODE::NONE)] = { "SmoothStep", "Quintic" };
    bool do_once = true;
    
    GLuint texobj = 0;
    glm::vec3 derivs = { 0,0,0 };
    float frequency = 0.1f;
    int current_item = 0;
    PERLIN_MODE curr_mode = PERLIN_MODE::SMOOTH;
};

