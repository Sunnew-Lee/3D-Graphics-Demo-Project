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
#include <glhelper.h>
#include <PerlinNoise.h>


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
            v.nrm = Vec3(v.pos.x, v.pos.y, 1.0f);

            v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh* CreateTerrain(int stacks, int slices, float& frequency)
{
    Mesh* mesh = new Mesh();
    mesh->stack_ = stacks;
    mesh->slice_ = slices;

    PerlinNoise* perlinnoise = new PerlinNoise(2016);

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;
            glm::vec3 derivs;
            float pos_z = (perlinnoise->eval4Quintic(Vec3(slice - 0.5f, 0.5f - stack, 0.0f) * frequency, derivs));

            v.pos = Vec3(col - 0.5f, 0.5f - row, pos_z);
            v.nrm = Vec3(derivs.x, derivs.y, -1);
            v.uv = Vec2(col, row);

            addVertex(*mesh, v);
        }
    }
    
    BuildIndexBuffer(stacks, slices, *mesh);
    delete perlinnoise;
    
    return mesh;
}

void Mesh::UpdateTerrain(double dt, float& frequency)
{
    this->vertexBuffer.clear();
    this->numVertices = 0;
    this->indexBuffer.clear();
    this->numIndices = 0;
    this->numTris = 0;
    this->x_pos += dt * 5;

    PerlinNoise* perlinnoise = new PerlinNoise(2016);

    int stacks = this->stack_;
    int slices = this->slice_;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;
            glm::vec3 derivs;
            float pos_z = (perlinnoise->eval4Quintic(Vec3(slice - 0.5f + this->x_pos, 0.5f - stack, 0.0f) * frequency, derivs));

            v.pos = Vec3(col - 0.5f, 0.5f - row, pos_z);
            v.nrm = Vec3(derivs.x, derivs.y, -1);
            v.uv = Vec2(col, row);

            addVertex(*this, v);
        }
    }

    BuildIndexBuffer(stacks, slices, *this);
    delete perlinnoise;
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
            vertex.nrm.y = -vertex.pos.y / 0.5f;
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
                vertex.nrm.y = -vertex.pos.y / 0.5f;
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

		vertex.nrm.x = vertex.pos.x / 0.5f;
		vertex.nrm.y = -vertex.pos.y / 0.5f;
		vertex.nrm.z = vertex.pos.z / 0.5f;

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
        int curr_row = i * stride;

        for (int j = 0; j < slices; ++j)
        {
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

/******************************************************************************/
void Mesh::setup_shdrpgm(std::vector<std::pair<GLenum, std::string>>& shdr_files)
{
    //std::vector<std::pair<GLenum, std::string>> shdr_files;
    //shdr_files.push_back(std::make_pair(
    //    GL_VERTEX_SHADER,
    //    "../shaders/model_shader.vert"));
    //shdr_files.push_back(std::make_pair(
    //    GL_FRAGMENT_SHADER,
    //    "../shaders/model_shader.frag"));
    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Mesh::setup_mesh()
{
    shdr_pgm.Use();

    /*  Obtain the locations of the variables in the shaders with the given names */
    mvpMatLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "mvpMat");
    modelLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "Model");
    colorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "color");
    lightPosLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "u_light");
    sin_valLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "sin");
    shrinkLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "shrink");
    centerLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "center");
    heightLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "height");
    uColorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "uColor");
    InnerLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "TessLevelInner");
    OuterLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "TessLevelOuter");

    SendVertexData();

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*  Initially drawing using filled mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*  Hidden surface removal */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */

    shdr_pgm.UnUse();
}

void Mesh::compute_matrix([[maybe_unused]] float delta_time, glm::highp_ivec3 eye, glm::mat4 frustum)
{
    // eye = (eyeAlpha , eyeBeta, eyeRadius);
	/*  Update view transform matrix */
    shdr_pgm.Use();

	viewMat = LookAtOrigin(1.0f * eye.z, ONE_STEP * eye.x, ONE_STEP * eye.y);

	/*  Update projection matrix */
	projMat = frustum;

	/*  Update view/projection-related matrices for non-animating objects */

	MVPMat = projMat * viewMat * selfMat;

   
    shdr_pgm.UnUse();
}

void Mesh::init(std::vector<std::pair<GLenum, std::string>>& shdr_files, glm::vec4 selfcol, glm::vec3 pos, glm::vec3 scal, glm::vec3 rotate)
{
    position = pos;
    scale = scal;
    rotation = rotate;
    selfColor = selfcol;
    selfMat = Translate(pos) */*Rotate(HALF_PI, XAXIS) */  Scale(scal);
    setup_shdrpgm(shdr_files);
    setup_mesh();

}



void Mesh::draw(/*glm::vec3 color ,glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos, glm::vec3 view_pos*/)
{
    //glm::mat4 model = {
    //    1,0,0,0,
    //    0,1,0,0,
    //    0,0,1,0,
    //    0,0,0,1
    //};
    //model = glm::translate(model, position);
    //model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::scale(model, { scale.x,scale.y,scale.z });
    //glUniform4fv(colorLoc, 1, ValuePtr(color));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniform3fv(LightLoc, 1, ValuePtr(light_pos));
    //glUniform3fv(ViewPosLoc, 1, ValuePtr(view_pos));

    /*  Use obj's color if drawing wireframes or objs that don't have textures */

    shdr_pgm.Use();
    
    glm::vec4 useNormal{ -1.0f, -1.0f, -1.0f, 1.0f };

    if (GLHelper::currRenderMode == GLHelper::RenderMode::NORMAL)
    {
        glUniform4fv(colorLoc, 1, ValuePtr(selfColor));
    }

    else if (GLHelper::currRenderMode == GLHelper::RenderMode::WIREFRAME)
    {
        glUniform4fv(colorLoc, 1, ValuePtr(selfColor));
    }
   
    /*  Send MVP matrix to shaders */
    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, ValuePtr(glm::translate(MVPMat, position)));


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, ValuePtr(glm::translate(selfMat, position)));
    glUniform3fv(lightPosLoc, 1, ValuePtr(lightPos));
    glUniform1f(shrinkLoc, shrink);
    glUniform1f(centerLoc, center);
    glUniform1f(heightLoc, grassHeight);
    glUniform3fv(uColorLoc, 1, ValuePtr(uColor));

    //    /*  Tell shader to use obj's VAO for rendering */
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

    shdr_pgm.UnUse();
}

void Mesh::update_tess(int& eye)
{
    shdr_pgm.Use();

    if (ImGui::SliderFloat("Inner", &Inner, 1.0f, 5.0f))
    {
        glUniform1f(InnerLoc, Inner);
    }
    if (ImGui::SliderFloat("Outer", &Outer, 1.0f, 5.0f))
    {
        glUniform1f(InnerLoc, Outer);
    }
    ImGui::SliderInt("Depth", &eye, 1, 64);
    ImGui::SliderFloat("Shrink", &shrink, 0.00f, 1.00f);
    ImGui::SliderFloat("CenterLocation", &center, 0.25f, 1.00f);
    ImGui::SliderFloat("GrassHeight", &grassHeight, 1.0f, 10.0f);
    ImGui::SliderFloat3("GrassColor", &uColor.x, 0.f, 1.f);

    shdr_pgm.UnUse();
}

void Mesh::SendVertexData()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER,
        numVertices * vertexSize, &vertexBuffer[0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        numIndices * indexSize, &indexBuffer[0],
        GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, vLayout[i].normalized, vertexSize, reinterpret_cast<void*>(vLayout[i].offset));
    }
}

void Mesh::UpdateVertexData()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER,
        numVertices * vertexSize, &vertexBuffer[0],
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        numIndices * indexSize, &indexBuffer[0],
        GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, vLayout[i].normalized, vertexSize, reinterpret_cast<void*>(vLayout[i].offset));
    }
}