/******************************************************************************/
/*!
\file   mesh.cpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le, sunwoo.lee, jaewoo.choi
\par    Email: sang.le\@digipen.edu, sunwoo.lee@digipen.edu, jaewoo.choi@digipen.edu
\par    DigiPen login: sang.le, sunwoo.lee, jaewoo.choi
\date   03.17.2022
*/
/******************************************************************************/

#include <mesh.hpp>
#include <cmath>
#include <random>
#include <functional>


/*  Function prototype(s) */
void BuildIndexBuffer(int stacks, int slices, Mesh& mesh);
void addVertex(Mesh& mesh, const Vertex& v);
void addIndex(Mesh& mesh, int index);


/******************************************************************************/
/*!
\fn     Mesh CreatePlane(int stacks, int slices)
\brief
        Create a plane of dimension 1*1.
        The plane x/y-coordinates range from -0.5 to 0.5.
        The plane z-coordinate is 0.
\param  stacks
        Number of stacks on the plane
\param  slices
        Number of slices on the plane
\return
        The generated plane
*/
/******************************************************************************/
Mesh CreatePlane(int stacks, int slices)
{
    Mesh mesh;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;

            v.pos = Vec3(col - 0.5f, 0.5f - row, 0.0f);
            v.nrm = Vec3(v.pos.x, v.pos.y, -1.0f);
            v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh CreateTerrain(int stacks, int slices, double dt)
{
    x_pos += dt/2;
    Mesh mesh;
    PerlinNoise perlinnoise(2016);

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;
            glm::vec3 derivs;
            float pos_z = (perlinnoise.eval(Vec3(col - 0.5f+ x_pos, 0.5f - row, 0.0f) * 3.f, derivs));

            v.pos = Vec3(col - 0.5f , 0.5f - row, pos_z);
            v.nrm = Vec3(derivs.x, derivs.y, -1);
            v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

/******************************************************************************/
/*!
\fn     Mesh CreateCube(int stacks, int slices)
\brief
        Create a cube of dimension 1*1*1.
        The x/y/z-coordinates range from -0.5 to 0.5.
\param  stacks
        Number of stacks on each side of the cube
\param  slices
        Number of slices on each side of the cube
\return
        The generated cube
*/
/******************************************************************************/
Mesh CreateCube(int stacks, int slices)
{
    Mesh planeMesh = CreatePlane(stacks, slices);
    Mesh mesh;

    Vec3 const translateArray[] =
    {
        Vec3(+0.0f, +0.0f, +0.5f), // Z+
        Vec3(+0.0f, +0.0f, -0.5f), // Z-
        Vec3(+0.5f, +0.0f, +0.0f), // X+
        Vec3(-0.5f, +0.0f, +0.0f), // X-
        Vec3(+0.0f, +0.5f, +0.0f), // Y+
        Vec3(+0.0f, -0.5f, +0.0f), // Y-
    };

    Vec2 const rotateArray[] =
    {
        Vec2(+0.0f, +0.0f),             // Z+
        Vec2(+0.0f, (float)+PI),        // Z-
        Vec2(+0.0f, (float)+HALF_PI),   // X+       
        Vec2(+0.0f, (float)-HALF_PI),   // X-
        Vec2((float)-HALF_PI, +0.0f),   // Y+
        Vec2((float)+HALF_PI, +0.0f)    // Y-
    };


    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        Mat4 transformMat = Translate(translateArray[i]) *
            Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *
            Rotate(rotateArray[i][XINDEX], { 1,0,0 });

        for (int j = 0; j < planeMesh.numVertices; ++j)
        {
            Vertex v;
            v.pos = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].pos, 1.0));
            v.nrm = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
            v.uv = planeMesh.vertexBuffer[j].uv;

            v.pos = RoundDecimal(v.pos);
            v.nrm = RoundDecimal(v.nrm);
            v.pos *= -1.f;
            addVertex(mesh, v);
        }

        for (int j = 0; j < planeMesh.numIndices; ++j)
            addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
    }

    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateSphere(int stacks, int slices)
\brief
        Create a sphere of radius 0.5.
        The center of the sphere is (0, 0, 0).
\param  stacks
        Number of stacks on the sphere
\param  slices
        Number of slices on the sphere
\return
        The generated sphere
*/
/******************************************************************************/
Mesh CreateSphere(int stacks, int slices)
{
    Mesh mesh;

    float radius = 0.5;
    for (int i = 0; i <= stacks; i++)
    {
        float row = static_cast<float>(i) / stacks;
        float beta = PI * (row - 0.5f);
        for (int j = 0; j <= slices; j++)
        {
            Vertex vertex;
            float col = static_cast<float>(j) / slices;
            vertex.uv.x = -col;
            vertex.uv.y = -row;

            float alpha = col * PI * 2.0f;
            vertex.pos.x = radius * sin(alpha) * cos(beta);
            vertex.pos.y = radius * sin(beta);
            vertex.pos.z = radius * cos(alpha) * cos(beta);

            vertex.nrm.x = vertex.pos.x;
            vertex.nrm.y = vertex.pos.y;
            vertex.nrm.z = vertex.pos.z;
            vertex.nrm /= radius;
            addVertex(mesh, vertex);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateCylinder(int stacks, int slices)
\brief
        Create a cylinder with radius 0.5 and height 1.
        The center of the cylinder is (0, 0, 0).
        The cylinder mesh should also contain two caps.
\param  stacks
        Number of stacks on the cylinder
\param  slices
        Number of slices on the cylinder
\return
        The generated cylinder
*/
/******************************************************************************/
Mesh CreateCylinder(int stacks, int slices)
{
    Mesh mesh;
    Vertex vertex;

    /// For the body

    for (int i = 0; i <= stacks; i++)
    {
        float row = static_cast<float>(i) / stacks;

        for (int j = 0; j <= slices; j++)
        {
          	float col = static_cast<float>(j) / slices;
			float alpha = col * 2.0f * PI;
			float sinAlpha = sin(alpha);
			float cosAlpha = cos(alpha);

			vertex.uv.x = row;
			vertex.uv.y = col;

			vertex.pos.x = 0.5f * sinAlpha;
			vertex.pos.y = row - 0.5f;
			vertex.pos.z = 0.5f * cosAlpha;

			vertex.nrm.x = vertex.pos.x / 0.5f;
			vertex.nrm.y = 0.f;
			vertex.nrm.z = vertex.pos.z / 0.5f;

			addVertex(mesh, vertex);
		}
    }
    BuildIndexBuffer(stacks, slices, mesh);

    int vertex_size = static_cast<int>(mesh.vertexBuffer.size());

	/// For the caps

	for (int i = 0; i <= stacks; i++)
	{
        if (i == 0)
        {
            vertex.pos = Vec3(0.0, 0.5, 0.0);
            vertex.nrm.x = vertex.pos.x / 0.5f;
            vertex.nrm.y = vertex.pos.y / 0.5f;
            vertex.nrm.z = vertex.pos.z / 0.5f;
        }
        else if (i == 1)
        {
            vertex.pos = Vec3(0.0, -0.5, 0.0);
            vertex.nrm.x = vertex.pos.x / 0.5f;
            vertex.nrm.y = vertex.pos.y / 0.5f;
            vertex.nrm.z = vertex.pos.z / 0.5f;
        }

		addVertex(mesh, vertex);

		float row = static_cast<float>(i) / stacks;

		for (int j = 0; j <= slices; j++)
        {
			float col = static_cast<float>(j) / slices;
			float alpha = col * 2.0f * PI;
			float sinAlpha = sin(alpha);
			float cosAlpha = cos(alpha);

            vertex.uv.x = row;
            vertex.uv.y = col;

            if (i == 0)
            {
                vertex.pos = Vec3(0.5 * sinAlpha, 0.5, 0.5 * cosAlpha);

                vertex.nrm.x = vertex.pos.x / 0.5f;
                vertex.nrm.y = vertex.pos.y / 0.5f;
                vertex.nrm.z = vertex.pos.z / 0.5f;
            }
            else if (i == 1)
            {
                vertex.pos = Vec3(0.5 * sinAlpha, -0.5, 0.5 * cosAlpha);

                vertex.nrm.x = vertex.pos.x / 0.5f;
                vertex.nrm.y = vertex.pos.y / -0.5f;
                vertex.nrm.z = vertex.pos.z / 0.5f;
            }

			addVertex(mesh, vertex);
		}
	}

    for (int i = 0; i <= stacks; i++)
    {
        vertex_size += (i * (slices + 2));

        for (int j = 1; j < slices; j++)
        {
            addIndex(mesh, vertex_size);
            addIndex(mesh, vertex_size + j);
            addIndex(mesh, vertex_size + j + 1);
        }
        addIndex(mesh, vertex_size);
        addIndex(mesh, vertex_size + slices);
        addIndex(mesh, vertex_size + 1);
    }
    
    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
\brief
        Create a torus that starts from startAngle and ends at endAngle.
        The major and minor radii are 0.35 and 0.15, respectively.
        The center of the torus is (0, 0, 0).
\param  stacks
        Number of stacks on the torus
\param  slices
        Number of slices on the torus
\param  startAngle
        The starting angle of the torus
\param  endAngle
        The ending angle of the torus
\return
        The generated torus
*/
/******************************************************************************/
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
{
    Mesh mesh;

    float R = 0.3f;
    float r = 0.1f;

    for (int i = 0; i <= stacks; i++)
    {
        float row = static_cast<float>(i) / stacks;
        float alpha = (endAngle - startAngle) * row + startAngle + PI;
        float sinAlpha = sin(alpha);
        float cosAlpha = cos(alpha);
        for (int j = 0; j <= slices; j++)
        {
            Vertex vertex;
            float col = static_cast<float>(j) / slices;
            float beta = PI * col * 2.0f;
            vertex.uv.x = row;
            vertex.uv.y = col;


            vertex.pos.x = R * sinAlpha + r * cos(beta) * sinAlpha;
            vertex.pos.y = r * sin(beta);
            vertex.pos.z = (R + r * cos(beta)) * cosAlpha;
            Vec3 c = Vec3(R * sinAlpha, 0, R * cosAlpha);

            vertex.nrm = vertex.pos - c;
            vertex.nrm /= r;

            vertex.pos /= -2 * (R + r);


            addVertex(mesh, vertex);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateCone(int stacks, int slices)
\brief
        Create a cone with base radius 0.5 and height 1.
        The center along the axis of the cone is (0, 0, 0).
        The cone mesh should also contain its base.
\param  stacks
        Number of stacks on the cone
\param  slices
        Number of slices on the cone
\return
        The generated cone
*/
/******************************************************************************/
Mesh CreateCone(int stacks, int slices)
{//16 8
    Mesh mesh;
    Vertex vertex;

    float R = 0.5;

    for (int i = 0; i <= stacks; i++)
    {
        float row = static_cast<float>(i) / stacks;

        for (int j = 0; j <= slices; j++)
        {
            float col = static_cast<float>(j) / slices;
            float alpha = col * 2.0f * PI;
            float sinAlpha = sin(alpha);
            float cosAlpha = cos(alpha);

            vertex.uv.x = row;
            vertex.uv.y = col;

            vertex.pos.x = R * (0.5f - (row - 0.5f)) * sinAlpha;
            vertex.pos.y = (row - 0.5f);
            vertex.pos.z = R * (0.5f - (row - 0.5f)) * cosAlpha;

            vertex.nrm.x = vertex.pos.x / 0.5f;
            vertex.nrm.y = 0.f;
            vertex.nrm.z = vertex.pos.z / 0.5f;

            addVertex(mesh, vertex);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);

	int vertex_size = static_cast<int>(mesh.vertexBuffer.size());

	vertex.pos = Vec3(0.0, -0.5, 0.0);
	vertex.nrm = Vec3(0.0, -1.0, 0.0);

	addVertex(mesh, vertex);


	for (int j = 0; j <= slices; j++)
	{
		float col = static_cast<float>(j) / slices;
		float alpha = col * 2.0f * PI;
		float sinAlpha = sin(alpha);
		float cosAlpha = cos(alpha);

		vertex.uv.x = 0.;
		vertex.uv.y = col;

		vertex.pos = Vec3(0.5 * sinAlpha, -0.5, 0.5 * cosAlpha);

		vertex.nrm.x = 0.0;
		vertex.nrm.y = -1.0;
		vertex.nrm.z = 0.0;

		addVertex(mesh, vertex);
	}



	for (int j = 1; j < slices; j++)
	{
		addIndex(mesh, vertex_size);
		addIndex(mesh, vertex_size + j);
		addIndex(mesh, vertex_size + j + 1);
	}

	addIndex(mesh, vertex_size);
	addIndex(mesh, vertex_size + slices);
	addIndex(mesh, vertex_size + 1);


	return mesh;
}


/******************************************************************************/
/*!
\fn     void BuildIndexBuffer(int stacks, int slices, Mesh &mesh)
\brief
        Generate the index buffer for the mesh.
\param  stacks
        Number of stacks on the mesh
\param  slices
        Number of slices on the mesh
\param  mesh
        The mesh whose index buffer will be generated.
*/
/******************************************************************************/
void BuildIndexBuffer(int stacks, int slices, Mesh& mesh)
{
    int p0 = 0, p1 = 0, p2 = 0;
    int p3 = 0, p4 = 0, p5 = 0;
    int stride = slices + 1;
    for (int i = 0; i < stacks; ++i)
    {
        

        for (int j = 0; j < slices; ++j)
        {
            int curr_row = i * stride;
            /*  You need to compute the indices for the first triangle here */
            /*  ... */
            p0 = curr_row + j;
            p1 = p0 + 1;
            p2 = p1 + stride;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(mesh.vertexBuffer[p0].pos,
                mesh.vertexBuffer[p1].pos,
                mesh.vertexBuffer[p2].pos))
            {
                /*  Add the indices for the first triangle */
                addIndex(mesh, p0);
                addIndex(mesh, p1);
                addIndex(mesh, p2);
            }
            
            /*  You need to compute the indices for the second triangle here */
            /*  ... */
            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(mesh.vertexBuffer[p3].pos,
                mesh.vertexBuffer[p4].pos,
                mesh.vertexBuffer[p5].pos))
            {
                /*  Add the indices for the second triangle */
                addIndex(mesh, p3);
                addIndex(mesh, p4);
                addIndex(mesh, p5);
            }
        }
    }
}


/******************************************************************************/
/*!
\fn     void addVertex(Mesh &mesh, const Vertex &v)
\brief
        Add a vertex to a mesh
\param  mesh
        The mesh to be updated.
\param  v
        The vertex to be added.
*/
/******************************************************************************/
void addVertex(Mesh& mesh, const Vertex& v)
{
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}


/******************************************************************************/
/*!
\fn     void addIndex(Mesh &mesh, int index)
\brief
        Add an index to a mesh
\param  mesh
        The mesh to be updated.
\param  index
        The vertex index to be added.
*/
/******************************************************************************/
void addIndex(Mesh& mesh, int index)
{
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;

    if (mesh.numIndices % 3 == 0)
        ++mesh.numTris;
}

PerlinNoise::PerlinNoise(const unsigned& seed)
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
    for (unsigned i = 0; i < tableSize; ++i) {
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * PI;

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

float PerlinNoise::eval(const glm::vec3& p, glm::vec3& derivs) const
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

    derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
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
}
