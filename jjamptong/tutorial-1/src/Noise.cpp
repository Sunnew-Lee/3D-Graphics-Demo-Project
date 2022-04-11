#include <cstdio> 
#include <random> 
#include <functional> 
#include <iostream> 
#include <fstream> 
#include <cmath>
#include <array>

#include <glslshader.h>
#include <glhelper.h>
#include <glm/gtc/noise.hpp>

#include "Noise.h"

#include "IG.h"


GLuint texobj;

ValueNoise::ValueNoise(unsigned seed )
{
    std::mt19937 gen(seed);
    std::uniform_real_distribution distrFloat;
    auto randFloat = std::bind(distrFloat, gen);

    // create an array of random values and initialize permutation table
    for (unsigned k = 0; k < kMaxTableSize; ++k) {
        r[k] = randFloat();
        permutationTable[k] = k;
    }

    // shuffle values of the permutation table
    std::uniform_int_distribution distrUInt;
    auto randUInt = std::bind(distrUInt, gen);
    for (unsigned k = 0; k < kMaxTableSize; ++k) {
        unsigned i = randUInt() & kMaxTableSizeMask;
        std::swap(permutationTable[k], permutationTable[i]);
        permutationTable[k + kMaxTableSize] = permutationTable[k];
    }
}

float ValueNoise::eval(Vec2f const& p) const
{
    int xi = std::floor(p.x);
    int yi = std::floor(p.y);

    float tx = p.x - xi;
    float ty = p.y - yi;

    int rx0 = xi & kMaxTableSizeMask;
    int rx1 = (rx0 + 1) & kMaxTableSizeMask;
    int ry0 = yi & kMaxTableSizeMask;
    int ry1 = (ry0 + 1) & kMaxTableSizeMask;

    // random values at the corners of the cell using permutation table
    const float& c00 = r[permutationTable[permutationTable[rx0] + ry0]];
    const float& c10 = r[permutationTable[permutationTable[rx1] + ry0]];
    const float& c01 = r[permutationTable[permutationTable[rx0] + ry1]];
    const float& c11 = r[permutationTable[permutationTable[rx1] + ry1]];

    // remapping of tx and ty using the Smoothstep function 
    float sx = quintic(tx);
    float sy = quintic(ty);

    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    // linearly interpolate the nx0/nx1 along they y axis
    return lerp(nx0, nx1, sy);
}

void ValueNoise::init()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);


    GLint w = GLHelper::width, h = GLHelper::height;
    glViewport(0, 0, w, h);

    mesh_setup();
    texture_setup();

    IG::init();
}

void ValueNoise::makePPM()
{
    //GLuint width{ 256 }, height{ 256 };
    float* noiseMap = new float[GLHelper::width * GLHelper::height];
    
#if 0 
    // generate white noise
    unsigned seed = 2016;
    std::mt19937 gen(seed);
    std::uniform_real_distribution distr;
    auto dice = std::bind(distr, gen); // std::function<float()> 

    for (unsigned j = 0; j < GLHelper::height; ++j) {
        for (unsigned i = 0; i < GLHelper::width; ++i) {
            // generate a float in the range [0:1]
            noiseMap[j * GLHelper::width + i] = dice();
        }
    }
#else 
    // generate value noise
    ValueNoise noise;
    float frequency = 0.5f;
    for (unsigned j = 0; j < 256; ++j)
    {
        for (unsigned i = 0; i <256*3; ++i)
        {
            // generate a float in the range [0:1]
            ptr_texels[j][i] =static_cast<unsigned char>(noise.eval(Vec2f(i/3, j) * frequency) * 255.f);
        }
    }
#endif 

   // output noise map to PPM
   std::ofstream ofs;
   ofs.open("../images/noise.ppm", std::ios::out | std::ios::binary);
   ofs << "P6\n" << GLHelper::width << " " << GLHelper::height << "\n255\n";
   for (unsigned k = 0; k < GLHelper::width * GLHelper::height; ++k) {
       unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
       ofs << n << n << n;
   }
   ofs.close();

   delete[] noiseMap;

}

void ValueNoise::mesh_setup()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/noise.vert"));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/noise.frag"));
    shdr.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::array<glm::vec2, 4> pos_vtx{
    glm::vec2(0.5f, -0.5f), glm::vec2(0.5f,0.5f),
    glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, -0.5f)
    };
    std::array<glm::vec3, 4> clr_vtx{
    glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f)
    };
    std::array<glm::vec2, 4> tex_vtx{
    glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f),
    glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f)
    };
    vertices.push_back({ pos_vtx[0], clr_vtx[0],tex_vtx[3] });
    vertices.push_back({ pos_vtx[1], clr_vtx[1] ,tex_vtx[2] });
    vertices.push_back({ pos_vtx[2], clr_vtx[2] ,tex_vtx[1] });
    vertices.push_back({ pos_vtx[3], clr_vtx[3] ,tex_vtx[0] });

    std::array<GLushort, 4> idx_vtx{
        0,1,3,2
    };

    indices.push_back(idx_vtx[0]);
    indices.push_back(idx_vtx[1]);
    indices.push_back(idx_vtx[2]);
    indices.push_back(idx_vtx[3]);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::color));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texcoords));

    glBindVertexArray(0);
}

GLuint ValueNoise::texture_setup()
{
    GLuint width{ 256 }, height{ 256 }, bytes_per_texel{ 4 };
	//char* ptr_texels[256][256 * 3];
 //   std::ifstream is("../images/noise.ppm", std::ifstream::binary);
 //   if (is) 
 //   {
 //       is.seekg(0, is.end);
 //       int length = static_cast<int>(is.tellg());
 //       is.seekg(0, is.beg);
 //       ptr_texels = new char[length];
 //       is.read(ptr_texels, length);
 //       is.close();
 //   }
 //   else
 //   {
 //       std::cout << "error to open texture binary file";
 //   }

   
    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr_texels);
    glGenerateMipmap(GL_TEXTURE_2D);

    //delete[] ptr_texels;
    return texobj;
}

void ValueNoise::draw()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    shdr.Use();

   // // draw Mesh
    glBindVertexArray(VAO);
    glBindTextureUnit(6, texobj);
    GLuint tex_loc = glGetUniformLocation(shdr.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 6);

    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    shdr.UnUse();
    IG::draw();
}

void ValueNoise::update(double )
{
    IG::update();
}

void ValueNoise::cleanUp()
{
    IG::cleanup();
}