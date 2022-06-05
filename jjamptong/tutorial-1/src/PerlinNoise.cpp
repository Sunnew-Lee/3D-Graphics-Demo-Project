// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <random>
#include <functional>
#include <array>
#include <PerlinNoise.h>
#include <math.hpp>



PerlinNoise::PerlinNoise(const unsigned& seed ) 
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
	for (unsigned i = 0; i < tableSize; ++i) {

		// better
		float theta = acos(2 * dice() - 1);
		float phi = static_cast<float>(2 * dice() * PI);

		float x = cos(phi) * sin(theta);
		float y = sin(phi) * sin(theta);
		float z = cos(theta);
		gradients[i] = glm::vec3(x, y, z);

		permutationTable[i] = i;
	}

    std::uniform_int_distribution<unsigned> distributionInt;
    auto diceInt = std::bind(distributionInt, generator);
    // create permutation table
    for (unsigned i = 0; i < tableSize; ++i)
        std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
    // extend the permutation table in the index range [256:512]
    for (unsigned i = 0; i < tableSize; ++i) {
        permutationTable[tableSize + i] = permutationTable[i];
    }
}

uint8_t PerlinNoise::hash(const int& x, const int& y, const int& z) const
{
	return static_cast<uint8_t>(permutationTable[permutationTable[permutationTable[x] + y] + z]);
}


float PerlinNoise::eval4SmoothStep(const glm::vec3& p) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = smoothstep(tx);
    float v = smoothstep(ty);
    float w = smoothstep(tz);

    // gradients at the corner of the cell
    const glm::vec3& c000 = gradients[hash(xi0, yi0, zi0)];
    const glm::vec3& c100 = gradients[hash(xi1, yi0, zi0)];
    const glm::vec3& c010 = gradients[hash(xi0, yi1, zi0)];
    const glm::vec3& c110 = gradients[hash(xi1, yi1, zi0)];

    const glm::vec3& c001 = gradients[hash(xi0, yi0, zi1)];
    const glm::vec3& c101 = gradients[hash(xi1, yi0, zi1)];
    const glm::vec3& c011 = gradients[hash(xi0, yi1, zi1)];
    const glm::vec3& c111 = gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    glm::vec3 p000 = glm::vec3(x0, y0, z0);
    glm::vec3 p100 = glm::vec3(x1, y0, z0);
    glm::vec3 p010 = glm::vec3(x0, y1, z0);
    glm::vec3 p110 = glm::vec3(x1, y1, z0);

    glm::vec3 p001 = glm::vec3(x0, y0, z1);
    glm::vec3 p101 = glm::vec3(x1, y0, z1);
    glm::vec3 p011 = glm::vec3(x0, y1, z1);
    glm::vec3 p111 = glm::vec3(x1, y1, z1);

    // linear interpolation
    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w); // g 
}

float PerlinNoise::gradientDotV(uint8_t perm, float x, float y, float z) const
{
    switch (perm & 15) {
    case  0: return  x + y; // (1,1,0) 
    case  1: return -x + y; // (-1,1,0) 
    case  2: return  x - y; // (1,-1,0) 
    case  3: return -x - y; // (-1,-1,0) 
    case  4: return  x + z; // (1,0,1) 
    case  5: return -x + z; // (-1,0,1) 
    case  6: return  x - z; // (1,0,-1) 
    case  7: return -x - z; // (-1,0,-1) 
    case  8: return  y + z; // (0,1,1), 
    case  9: return -y + z; // (0,-1,1), 
    case 10: return  y - z; // (0,1,-1), 
    case 11: return -y - z; // (0,-1,-1) 
    case 12: return  y + x; // (1,1,0) 
    case 13: return -x + y; // (-1,1,0) 
    case 14: return -y + z; // (0,-1,1) 
    case 15: return -y - z; // (0,-1,-1) 
    }

    return 0.f;
}

float PerlinNoise::eval4Quintic(const glm::vec3& p, glm::vec3& divs) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = quinticDeriv(tx);
    float dv = quinticDeriv(ty);
    float dw = quinticDeriv(tz);

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    divs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    divs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    divs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;

}

// store perlin noise data in ptr_texels -> method smoothstep
void PerlinNoise::makePPM()
{

    for (uint32_t j = 0; j < 256; ++j) {
        for (uint32_t i = 0; i < 256 * 3; ++i) {
            ptr_texels[j][i] = static_cast<unsigned char>((eval4SmoothStep(glm::vec3(i / 3, 0, j) * frequency) + 1) * 0.5f * 255.f);
        }
    }
}

// store perlin noise data in ptr_texels -> method quintic
void PerlinNoise::makePPM4Quintic()
{
    for (uint32_t j = 0; j < 256; ++j) {
        for (uint32_t i = 0; i < 256 * 3; ++i) {
            ptr_texels[j][i] = static_cast<unsigned char>((eval4Quintic(glm::vec3(i / 3, 0, j) * frequency, derivs) + 1) * 0.5f * 255.f);
        }
    }
}

// set up shaders and vertex
void PerlinNoise::mesh_setup()
{
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

// set up texture
GLuint PerlinNoise::texture_setup()
{
    GLuint width{ 256 }, height{ 256 };

    if (do_once)
    {
        glGenTextures(1, &texobj);
        do_once = false;
    }
    glBindTexture(GL_TEXTURE_2D, texobj);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr_texels);
    glGenerateMipmap(GL_TEXTURE_2D);

    //delete[] ptr_texels;
    return texobj;
}

// set up perlin noise
void PerlinNoise::init()
{
   glClearColor(1.f, 1.f, 1.f, 1.f);
    
    glViewport(0, 0, GLHelper::width, GLHelper::height);

    shdr_setup();
    vert_setup();
    mesh_setup();
    makePPM();
    texture_setup();
}

// re-set up every time the frequency or method changes
void PerlinNoise::update([[maybe_unused]] double delta_time)
{
    if (ImGui::SliderFloat("frequency", &frequency, 0.0f, 0.2f))
    {
        if (curr_mode == PERLIN_MODE::SMOOTH) {
            makePPM();
        }
        else if (curr_mode == PERLIN_MODE::QUINTIC)
        {
            makePPM4Quintic();
        }
        texture_setup();
    }

    if (ImGui::Combo("Change Method", &current_item, items, IM_ARRAYSIZE(items)))
    {
        switch (current_item)
        {
        case 0:curr_mode = PERLIN_MODE::SMOOTH; makePPM(); texture_setup(); break;
        case 1:curr_mode = PERLIN_MODE::QUINTIC; makePPM4Quintic(); texture_setup(); break;

        default: curr_mode = PERLIN_MODE::SMOOTH; break;
        }
    }    
}  

// draw the data
void PerlinNoise::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shdr.Use();
    // // draw Mesh
    glBindVertexArray(VAO);
    glBindTextureUnit(6, texobj);
    GLuint tex_loc = glGetUniformLocation(shdr.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 6);

    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    shdr.UnUse();
}

void PerlinNoise::cleanup()
{
}

void PerlinNoise::vert_setup()
{
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
}

void PerlinNoise::shdr_setup()
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
}
