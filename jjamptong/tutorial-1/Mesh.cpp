/******************************************************************************/
/*!
\author Sang Le
*/
/******************************************************************************/

//Name & co-author: Sunwoo Lee, DongA Choi - special thanks(Hyosang Jung)
// Course Name: CS250
//Assignment Name: Procedural Modeling Demo
//Term&Year: 2022 Spring

#include "Mesh.h"

void BuildIndexBuffer(int stacks, int slices, Mesh& mesh);
void addVertex(Mesh& mesh, const Vertex& v);
void addIndex(Mesh& mesh, int index);


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

            v.pos = glm::vec3(col - 0.5f, 0.5f - row, 0.0f);
            v.nrm = glm::vec3(0.0f, 0.0f, 1.0f);
           // v.uv = glm::vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}


Mesh CreateCube(int stacks, int slices)
{
    Mesh planeMesh = CreatePlane(stacks, slices);
    Mesh mesh;

    glm::vec3 const translateArray[] =
    {
        glm::vec3(+0.0f, +0.0f, +0.5f), // Z+
        glm::vec3(+0.0f, +0.0f, -0.5f), // Z-
        glm::vec3(+0.5f, +0.0f, +0.0f), // X+
        glm::vec3(-0.5f, +0.0f, +0.0f), // X-
        glm::vec3(+0.0f, +0.5f, +0.0f), // Y+
        glm::vec3(+0.0f, -0.5f, +0.0f), // Y-
    };

    glm::vec2 const rotateArray[] =
    {
        glm::vec2(+0.0f, +0.0f),             // Z+
        glm::vec2(+0.0f, (float)+PI),        // Z-
        glm::vec2(+0.0f, (float)+HALF_PI),   // X+       
        glm::vec2(+0.0f, (float)-HALF_PI),   // X-
        glm::vec2((float)-HALF_PI, +0.0f),   // Y+
        glm::vec2((float)+HALF_PI, +0.0f)    // Y-
    };


    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        glm::mat4 transformMat = Translate(translateArray[i]) *
            Rotate(rotateArray[i][1], { 0,1,0 }) *
            Rotate(rotateArray[i][0], { 1,0,0 });

        for (int j = 0; j < planeMesh.numVertices; ++j)
        {
            Vertex v;
            v.pos = glm::vec3(transformMat * glm::vec4(planeMesh.vertexBuffer[j].pos, 1.0));
            v.nrm = glm::vec3(transformMat * glm::vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
            //v.uv = planeMesh.vertexBuffer[j].uv;

            v.pos = RoundDecimal(v.pos);
            v.nrm = RoundDecimal(v.nrm);

            addVertex(mesh, v);
        }

        for (int j = 0; j < planeMesh.numIndices; ++j)
            addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
    }

    return mesh;
}


Mesh CreateSphere(int stacks, int slices)
{
    Mesh mesh;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;
        float beta = PI * (row - 0.5f);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;
            float alpha = col * PI * 2.0f;
            Vertex v;
            /*v.uv.x = col;
            v.uv.y = row * (-1.0f);*/

            v.pos.x = 0.5f * sin(alpha) * cos(beta);
            v.pos.y = 0.5f * sin(beta);
            v.pos.z = 0.5f * cos(alpha) * cos(beta);

            v.nrm.x = v.pos.x;
            v.nrm.y = v.pos.y;
            v.nrm.z = v.pos.z;

            v.nrm /= 0.5;

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh CreateCylinder(int stacks, int slices)
{
    Mesh mesh;
    Mesh top;
    Mesh bottom;
    float col = 0;
    float alpha = 0;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks; // [0.0, 1.0]
        float z_val = 0.5;
        float diff = 2.0f * PI / slices;
        for (int slice = 0; slice <= slices; ++slice)
        {
            col = (float)slice / slices;
            alpha = col * PI * 2.0f;

            Vertex v;

            // side
            /*v.uv.x = row;
            v.uv.y = col;*/

            v.pos.x = 0.5f * sin(alpha);
            v.pos.y = row - 0.5f;
            v.pos.z = 0.5f * cos(alpha);


            v.nrm.x = v.pos.x / 0.5f;
            v.nrm.y = 0.0f;
            v.nrm.z = v.pos.z / 0.5f;

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    ////////////////////////////////////////////////

    addIndex(top, 0);
    addIndex(top, 1);
    addIndex(top, 2);

    addIndex(top, 0);
    addIndex(top, 2);
    addIndex(top, 3);

    addIndex(top, 0);
    addIndex(top, 3);
    addIndex(top, 4);

    addIndex(top, 0);
    addIndex(top, 4);
    addIndex(top, 5);

    addIndex(top, 0);
    addIndex(top, 5);
    addIndex(top, 6);

    addIndex(top, 0);
    addIndex(top, 6);
    addIndex(top, 7);

    addIndex(top, 0);
    addIndex(top, 7);
    addIndex(top, 8);

    addIndex(top, 0);
    addIndex(top, 8);
    addIndex(top, 1);

    for (int j = 0; j < top.numIndices; ++j)
        addIndex(mesh, mesh.numVertices + top.indexBuffer[j]);



    Vertex upper_center;
    upper_center.pos = glm::vec3{ 0,0.5,0 };
    addVertex(mesh, upper_center);

    for (int stack = 0; stack < stacks; ++stack)
    {
        float row = (float)stack / stacks; // [0.0, 1.0]
        float diff = 2.0f * PI / slices;

        for (int slice = 0; slice < slices; ++slice)
        {
            col = (float)slice / slices;
            alpha = col * PI * 2.0f;

            Vertex upper;

            /*upper.uv.x = row;
            upper.uv.y = col;*/

            upper.pos.x = 0.5f * sin(alpha);
            upper.pos.y = 0.5f;
            upper.pos.z = 0.5f * cos(alpha);

            upper.nrm.x = 0;
            upper.nrm.y = 1.0;
            upper.nrm.z = 0;

            addVertex(mesh, upper);
        }
    }



    //bottom

    addIndex(bottom, 0);
    addIndex(bottom, 1);
    addIndex(bottom, 2);

    addIndex(bottom, 0);
    addIndex(bottom, 2);
    addIndex(bottom, 3);

    addIndex(bottom, 0);
    addIndex(bottom, 3);
    addIndex(bottom, 4);

    addIndex(bottom, 0);
    addIndex(bottom, 4);
    addIndex(bottom, 5);

    addIndex(bottom, 0);
    addIndex(bottom, 5);
    addIndex(bottom, 6);

    addIndex(bottom, 0);
    addIndex(bottom, 6);
    addIndex(bottom, 7);

    addIndex(bottom, 0);
    addIndex(bottom, 7);
    addIndex(bottom, 8);

    addIndex(bottom, 0);
    addIndex(bottom, 8);
    addIndex(bottom, 1);

    for (int j = 0; j < bottom.numIndices; ++j)
        addIndex(mesh, mesh.numVertices + bottom.indexBuffer[j]);


    upper_center.pos = glm::vec3{ 0,-0.5,0 };
    addVertex(mesh, upper_center);

    for (int stack = 0; stack < stacks; ++stack)
    {
        float row = (float)stack / stacks; // [0.0, 1.0]
        float diff = 2.0f * PI / slices;

        for (int slice = 0; slice < slices; ++slice)
        {
            col = (float)slice / slices;
            alpha = col * PI * 2.0f;

            Vertex upper;

            /*upper.uv.x = row;
            upper.uv.y = col;*/

            upper.pos.x = 0.5f * sin(alpha);
            upper.pos.y = -0.5f;
            upper.pos.z = 0.5f * cos(alpha);

            upper.nrm.x = 0;
            upper.nrm.y = -1.0;
            upper.nrm.z = 0;

            addVertex(mesh, upper);
        }
    }



    return mesh;
}


Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
{
    Mesh mesh;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks; // [0.0, 1.0]

        float alpha = PI + startAngle + (row * (endAngle - startAngle)); // [0.0, 2¥ð]

        float sinAlpha = sin(alpha);
        float  cosAlpha = cos(alpha);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            float beta = col * PI * 2.0f;

            Vertex center;
            center.pos = glm::vec3(0, 0, 0);

            Vertex v;
            /*v.uv.x = col;
            v.uv.y = row;*/

            v.pos.x = -(0.35f + (0.15f * cos(beta))) * sinAlpha;
            v.pos.y = -0.15f * sin(beta);
            v.pos.z = -(0.35f + (0.15f * cos(beta))) * cosAlpha;

            v.nrm = v.pos - center.pos;

            v.nrm /= 0.15;
            addVertex(mesh, v);
        }

    }

    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}

Mesh CreateCone(int stacks, int slices)
{
    Mesh mesh;
    Mesh top;
    Mesh bottom;
    float col = 0;
    float alpha = 0;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = ((float)stack / stacks) - 0.5f; // [0.0, 1.0]
        float z_val = 0.5f;
        for (int slice = 0; slice <= slices; ++slice)
        {
            col = (float)slice / slices;
            alpha = col * PI * 2.0f;

            Vertex v;

            // side
            /*v.uv.x = row;
            v.uv.y = col;*/

            v.pos.x = 0.5f * (0.5f - row) * sin(alpha);
            v.pos.y = row;
            v.pos.z = 0.5f * (0.5f - row) * cos(alpha);


            v.nrm.x = v.pos.x / 0.5f;
            v.nrm.y = 0.0;
            v.nrm.z = v.pos.z / 0.5f;

            addVertex(mesh, v);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    return  mesh;
}

void BuildIndexBuffer(int stacks, int slices, Mesh& mesh)
{
    //@todo: IMPLEMENT ME
    int p0 = 0, p1 = 0, p2 = 0;
    int p3 = 0, p4 = 0, p5 = 0;

    int stride = slices + 1;
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int current_row = i * stride;
            /*  You need to compute the indices for the first triangle here */
            /*  ... */
            p0 = current_row + j;
            p1 = p0 + 1;
            p2 = p1 + stride;
            // triangle <p3, p4, p5>
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

void addVertex(Mesh& mesh, const Vertex& v)
{
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}

void addIndex(Mesh& mesh, int index)
{
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;

    if (mesh.numIndices % 3 == 0)
        ++mesh.numTris;
}