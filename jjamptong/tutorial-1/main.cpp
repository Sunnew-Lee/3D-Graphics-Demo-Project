#include <fstream>

#include <glhelper.h>
#include <glapp.h>

#include "Noise.h"

int main(int argc, char** argv)
{
    unsigned imageWidth = 512;
    unsigned imageHeight = 512;
    float* noiseMap = new float[imageWidth * imageHeight];
#if 0 
    // generate white noise
    unsigned seed = 2016;
    std::mt19937 gen(seed);
    std::uniform_real_distribution distr;
    auto dice = std::bind(distr, gen); // std::function<float()> 

    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            // generate a float in the range [0:1]
            noiseMap[j * imageWidth + i] = dice();
        }
    }
#else 
    // generate value noise
    ValueNoise noise;
    float frequency = 0.05f;
    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            // generate a float in the range [0:1]
            noiseMap[j * imageWidth + i] = noise.eval(Vec2f(i, j) * frequency);
        }
    }
#endif 

    // output noise map to PPM
    std::ofstream ofs;
    ofs.open("./noise.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
    for (unsigned k = 0; k < imageWidth * imageHeight; ++k) {
        unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
        ofs << n << n << n;
    }
    ofs.close();

    delete[] noiseMap;

    return 0;
}