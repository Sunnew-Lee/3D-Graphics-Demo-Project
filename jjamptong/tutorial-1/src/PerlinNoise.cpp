#include <array>
#include "PerlinNoise.h"
#include <IG.h>

GLuint texobj;
PerlinNoise noise;

Vec3f derivs;
static float frequency = 64.f;

PerlinNoise::PerlinNoise(const unsigned& seed )
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
    for (unsigned i = 0; i < tableSize; ++i) {
#if 0 
        // bad
        float gradientLen2;
        do {
            gradients[i] = Vec3f(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1);
            gradientLen2 = gradients[i].length2();
        } while (gradientLen2 > 1);
        gradients[i].normalize();
#else 
        // better
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * std::acos(-1);

        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
        gradients[i] = Vec3f(x, y, z);
#endif 
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

int PerlinNoise::hash(const int& x, const int& y, const int& z) const
{
	return permutationTable[permutationTable[permutationTable[x] + y] + z];
}


float PerlinNoise::eval(const Vec3f& p, Vec3f& derivs) const
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
    const Vec3f& c000 = gradients[hash(xi0, yi0, zi0)];
    const Vec3f& c100 = gradients[hash(xi1, yi0, zi0)];
    const Vec3f& c010 = gradients[hash(xi0, yi1, zi0)];
    const Vec3f& c110 = gradients[hash(xi1, yi1, zi0)];

    const Vec3f& c001 = gradients[hash(xi0, yi0, zi1)];
    const Vec3f& c101 = gradients[hash(xi1, yi0, zi1)];
    const Vec3f& c011 = gradients[hash(xi0, yi1, zi1)];
    const Vec3f& c111 = gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    Vec3f p000 = Vec3f(x0, y0, z0);
    Vec3f p100 = Vec3f(x1, y0, z0);
    Vec3f p010 = Vec3f(x0, y1, z0);
    Vec3f p110 = Vec3f(x1, y1, z0);

    Vec3f p001 = Vec3f(x0, y0, z1);
    Vec3f p101 = Vec3f(x1, y0, z1);
    Vec3f p011 = Vec3f(x0, y1, z1);
    Vec3f p111 = Vec3f(x1, y1, z1);

    // linear interpolation
    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w); // g 
}

void PerlinNoise::makePPM()
{

    // output noise map to PPM
    const uint32_t width = 512, height = 512;
    noiseMap = new float[width * height];

    for (uint32_t j = 0; j < 256; ++j) {
        for (uint32_t i = 0; i < 256 * 3; ++i) {
            ptr_texels[j][i] = (noise.eval(Vec3f(i / 3, 0, j) * (1 / frequency), derivs) + 1) * 0.5f * 255.f;
        }
    }

    /*std::ofstream ofs;
    ofs.open("../images/perlin.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned k = 0; k < width * height; ++k) {
        unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
        ofs << n << n << n;
    }
    ofs.close();*/

    delete[] noiseMap;
    
}

void PerlinNoise::mesh_setup()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;

    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/perlin.vert"));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/perlin.frag"));
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

GLuint PerlinNoise::texture_setup()
{
    GLuint width{ 256 }, height{ 256 }, bytes_per_texel{ 4 };

    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr_texels);
    glGenerateMipmap(GL_TEXTURE_2D);

    //delete[] ptr_texels;
    return texobj;
}

void PerlinNoise::init()
{
   // glClearColor(1.f, 1.f, 1.f, 1.f);

    GLint w = GLHelper::width, h = GLHelper::height;
    glViewport(0, 0, w, h);

    mesh_setup();
    texture_setup();

    IG::init();
}

void PerlinNoise::update(double)
{
    IG::update();
    if (ImGui::SliderFloat("frequency", &frequency, 2.0f, 64.0f))
    {
        //isChanged = true;
        mesh_setup();
        texture_setup();
        makePPM();
    }
  
}  

void PerlinNoise::draw()
{
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

void PerlinNoise::cleanup()
{
    IG::cleanup();
}
