#pragma once
#include <functional>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glhelper.h>
#include <random>

#include <glslshader.h>

template<typename T>
class Vec2
{
public:
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}
    Vec2 operator * (const T& r) const { return Vec2(x * r, y * r); }
    Vec2& operator *= (const T& r) { x *= r, y *= r; return *this; }
    T x, y;
};

template<typename T>
class Vec3
{
public:
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3 operator * (const T& r) const { return Vec3(x * r, y * r, z * r); }
    Vec3 operator - (const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3& operator *= (const T& r) { x *= r, y *= r, z *= r; return *this; }
    T length2() const { return x * x + y * y + z * z; }
    Vec3& operator /= (const T& r) { x /= r, y /= r, z /= r; return *this; }
    Vec3 cross(const Vec3& v) const
    {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    Vec3& normalize()
    {
        T len2 = length2();
        if (len2 > 0) {
            T invLen = T(1) / sqrt(len2);
            x *= invLen, y *= invLen, z *= invLen;
        }
        return *this;
    }
    friend std::ostream& operator << (std::ostream& os, const Vec3<T>& v)
    {
        os << v.x << ", " << v.y << ", " << v.z;
        return os;
    }
    T x, y, z;
};

typedef Vec2<float> Vec2f;
typedef Vec3<float> Vec3f;

template<typename T = float>
inline T dot(const Vec3<T>& a, const Vec3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T = float>
inline T lerp(const T& lo, const T& hi, const T& t)
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

inline float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

class PolyMesh
{
public:
    PolyMesh() : vertices(nullptr), st(nullptr), normals(nullptr) {}
    ~PolyMesh()
    {
        if (vertices) delete[] vertices;
        if (st) delete[] st;
        if (normals) delete[] normals;
    }
    Vec3f* vertices;
    Vec2f* st;
    Vec3f* normals;
    uint32_t* faceArray;
    uint32_t* verticesArray;
    uint32_t numVertices;
    uint32_t numFaces;
};

class PerlinNoise
{
public:
    PerlinNoise(const unsigned& seed = 2016);
    void makePPM();
    void makePPM4Quintic();
    virtual ~PerlinNoise() {}
    /* inline */
    
    void mesh_setup();
    GLuint texture_setup();
    void init();
    void update(double delta_time);
    void draw();
    void cleanup();

private:
    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    Vec3f gradients[tableSize];
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
    //unsigned char ptr_texels
    //float* noiseMap = nullptr;
	std::vector<GLushort> indices;

    int hash(const int& x, const int& y, const int& z) const;
    float eval(const Vec3f& p, Vec3f& derivs) const;

    float gradientDotV(uint8_t perm, float x, float y, float z) const;
    float eval4Quintic(const Vec3f& p, Vec3f& derivs) const;

    const char* items[2] = { "SmoothStep", "Quintic" };
    
    
};

