// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

#pragma once
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glslshader.h>
#include <glDemo.h>

class ValueNoise : public GLDemo
{
public:
    ValueNoise(unsigned seed = 2016);
    ~ValueNoise() override {};

    void init() override;
    void update(double delta_time) override;
    void draw() override;
    void cleanup() override;

    void vert_setup();
    void shdr_setup();
    float eval(glm::vec2 const& p) const;
    void makePPM();
    void makePPM4Marble();
    void makePPM4Wood();
    //void makePPM4Turbulence();
    void mesh_setup();
    GLuint texture_setup();    

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

    int numLayers = 5;
    float frequencyMult = 1.8f;
    float amplitudeMult = 0.35f;
    enum class VALUE_MODE
    {
        VALUE = 0,
        MARBLE,
        WOOD,
        NONE
    };
	enum class METHOD_MODE
    {
	    SMOOTHSTEP,
        QUINTIC,
        NONE
    };

    const char* items[static_cast<int>(VALUE_MODE::NONE)] = { "Value", "Marble", "Wood" };
    const char* methods[static_cast<int>(METHOD_MODE::NONE)] = { "SmoothStep", "Quintic" };
	bool do_once = true;

    mutable float sx = 0.f;
	mutable float sy = 0.f;
    
    GLuint texobj = 0;
    float frequency = 0.1f;
    float frequencyMarble = 0.02f;
    float frequencyWood = 0.01f;
    int current_item = 0;
    int current_method = 0;
    VALUE_MODE curr_mode = VALUE_MODE::VALUE;
    METHOD_MODE curr_method_mode = METHOD_MODE::QUINTIC;
};