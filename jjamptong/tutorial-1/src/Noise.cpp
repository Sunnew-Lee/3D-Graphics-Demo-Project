// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

#include <random> 
#include <functional> 
#include <array>
#include <Noise.h>
#include <math.hpp>

ValueNoise::ValueNoise(unsigned seed)
{
    std::mt19937 gen(seed);
    std::uniform_real_distribution distrFloat;
    auto randFloat = std::bind(distrFloat, gen);

    // create an array of random values and initialize permutation table
    for (unsigned k = 0; k < kMaxTableSize; ++k) {
        r[k] = static_cast<float>(randFloat());
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

float ValueNoise::eval(glm::vec2 const& p) const
{
    int xi = static_cast<int>(std::floor(p.x));
    int yi = static_cast<int>(std::floor(p.y));

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
    if (curr_method_mode == METHOD_MODE::QUINTIC) {
        sx = quintic(tx);
        sy = quintic(ty);
    }else if(curr_method_mode == METHOD_MODE::SMOOTHSTEP)
    {
        sx = smoothstep(tx);
        sy = smoothstep(ty);
    }
  
    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);

    // linearly interpolate the nx0/nx1 along they y axis
    return lerp(nx0, nx1, sy);
}

void ValueNoise::init()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glViewport(0, 0, GLHelper::width, GLHelper::height);

    shdr_setup();
    vert_setup();
    mesh_setup();
    makePPM();
    texture_setup();
}

void ValueNoise::makePPM()
{
    for (unsigned j = 0; j < 256; ++j)
    {
        for (unsigned i = 0; i < 256 * 3; ++i)
        {
            // generate a float in the range [0:1]
            ptr_texels[j][i] = static_cast<unsigned char>(eval(glm::vec2(i / 3, j) * frequency) * 255.f);
        }
    }

}


void ValueNoise::makePPM4Marble()
{
    for (unsigned j = 0; j < 256; ++j) {
        for (unsigned i = 0; i < 256 * 3; ++i) {
            glm::vec2 pNoise = glm::vec2(i / 3, j) * frequencyMarble;
            float amplitude = 1;
            float noiseValue = 0;
            // compute some fractal noise
            for (int l = 0; l < numLayers; ++l) {
                noiseValue += eval(pNoise) * amplitude;
                pNoise *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            // we "displace" the value i used in the sin() expression by noiseValue * 100
            ptr_texels[j][i] = static_cast<unsigned char>(((sin((i + noiseValue * 100) * 2 * PI / 200.f) + 1) / 2.f) * 255.f);
        }
    }
}

void ValueNoise::makePPM4Wood()
{
    for (unsigned j = 0; j < 256; ++j) {
        for (unsigned i = 0; i < 256 * 3; ++i) {
            float g = eval(glm::vec2(i / 3, j) * frequencyWood) * 10;
            ptr_texels[j][i] = static_cast<unsigned char>((g - static_cast<int>(g)) * 255.f);
        }
    }
}

//void ValueNoise::makePPM4Turbulence()
//{
//    ValueNoise noise;
//    float frequencyTur = 0.02f;
//    float frequencyMultTur = 1.8f;
//    float amplitudeMultTur = 0.35f;
//    unsigned numLayers = 5;
//    float maxNoiseVal = 0;
//    for (unsigned j = 0; j < 256; ++j) {
//        for (unsigned i = 0; i < 256 * 3; ++i) {
//            glm::vec2 pNoise = glm::vec2(i/3, j) * frequencyTur;
//            float amplitude = 1;
//            for (unsigned l = 0; l < numLayers; ++l) {
//                ptr_texels[j][i] += std::fabs(2 * noise.eval(pNoise) - 1) * amplitude;
//                pNoise *= frequencyMultTur;
//                amplitude *= amplitudeMultTur;
//            }
//            if (ptr_texels[j][ i] > maxNoiseVal) maxNoiseVal = ptr_texels[j][ i];
//        }
//    }
//    for (unsigned j = 0; j < 256; ++j) {
//        for (unsigned i = 0; i < 256*3 ; ++i) {
//            ptr_texels[j][i] /= maxNoiseVal;
//        }
//    }
//}

void ValueNoise::mesh_setup()
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

GLuint ValueNoise::texture_setup()
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

void ValueNoise::draw()
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

void ValueNoise::update([[maybe_unused]]double delta_time)
{
    // 슬라이더 조정시 업데이트
    // 모드 바꿨을때
    if (ImGui::Combo("Change Pattern", &current_item, items, IM_ARRAYSIZE(items)))
    {
        switch (current_item)
        {
        case 0:curr_mode = VALUE_MODE::VALUE; makePPM(); texture_setup(); break;
        case 1:curr_mode = VALUE_MODE::MARBLE; makePPM4Marble(); texture_setup(); break;
        case 2:curr_mode = VALUE_MODE::WOOD; makePPM4Wood(); texture_setup(); break;
        //case 3:curr_mode = VALUE_MODE::TURBULENCE; makePPM4Turbulence(); texture_setup(); break;
        default: curr_mode = VALUE_MODE::VALUE; break;
        }
    }

    if (curr_mode == VALUE_MODE::VALUE) {
        if (ImGui::SliderFloat("frequency", &frequency, 0.05f, 0.5f))
        {
            makePPM();
            texture_setup();
        }
        if (ImGui::Combo("Change Method", &current_method, methods, IM_ARRAYSIZE(methods)))
        {
            switch (current_method)
            {
            case 0:curr_method_mode = METHOD_MODE::SMOOTHSTEP; makePPM(); texture_setup(); break;
            case 1:curr_method_mode = METHOD_MODE::QUINTIC; makePPM(); texture_setup(); break;
            default: curr_method_mode = METHOD_MODE::QUINTIC; break;
            }
        }
    }
    else if (curr_mode == VALUE_MODE::MARBLE)
    {
        if (ImGui::SliderFloat("frequency", &frequencyMarble, 0.02f, 0.25f))
        {
            makePPM4Marble();
            texture_setup();
        }
        if(ImGui::SliderInt("numLayers", &numLayers, 1, 5))
        {
            makePPM4Marble();
            texture_setup();
        }if (ImGui::SliderFloat("frequencyMult", &frequencyMult, 0.5f, 2.5f))
        {
            makePPM4Marble();
            texture_setup();
        }
        if (ImGui::SliderFloat("amplitudeMult", &amplitudeMult, 0.f, 1.f))
        {
            makePPM4Marble();
            texture_setup();
        }
        if (ImGui::Combo("Change Method", &current_method, methods, IM_ARRAYSIZE(methods)))
        {
            switch (current_method)
            {
            case 0:curr_method_mode = METHOD_MODE::SMOOTHSTEP; makePPM4Marble(); texture_setup(); break;
            case 1:curr_method_mode = METHOD_MODE::QUINTIC; makePPM4Marble(); texture_setup(); break;
            default: curr_method_mode = METHOD_MODE::QUINTIC; break;
            }
        }
    }
    else if (curr_mode == VALUE_MODE::WOOD)
    {
        if (ImGui::SliderFloat("frequency", &frequencyWood, 0.01f, 0.25f))
        {
            makePPM4Wood();
            texture_setup();
        }
        if (ImGui::Combo("Change Method", &current_method, methods, IM_ARRAYSIZE(methods)))
        {
            switch (current_method)
            {
            case 0:curr_method_mode = METHOD_MODE::SMOOTHSTEP; makePPM4Wood(); texture_setup(); break;
            case 1:curr_method_mode = METHOD_MODE::QUINTIC; makePPM4Wood(); texture_setup(); break;
            default: curr_method_mode = METHOD_MODE::QUINTIC; break;
            }
        }
    }

    
   
}


void ValueNoise::vert_setup()
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

void ValueNoise::shdr_setup()
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

void ValueNoise::cleanup()
{
}
