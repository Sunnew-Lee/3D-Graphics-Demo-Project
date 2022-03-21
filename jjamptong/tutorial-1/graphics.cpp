/******************************************************************************/
/*!
\file   graphics.cpp
\par    Purpose: Implement the graphics-related functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#include "graphics.h"
//#include "textfile.h"
//#include "image_io.h"

#include <iostream>
#include <ctime>


/******************************************************************************/
/*  Graphics-related variables                                                */
/******************************************************************************/
/*  Viewport width & height */
int width = 800;
int height = 600;

/*  For animating the objects */
clock_t startTime;      /*  The starting time of the program */
clock_t currTime;       /*  We'll use (currTime-startTime) to avoid accumulated imprecision */
clock_t idleTime = 0;   /*  Amount of time animation is paused, so we can keep the animation smooth */
clock_t tempTime;
float secondsLapsed;        /*  Num of seconds from startTime to currTime */
bool animated = true;
bool justAnimated = false;  /*  To check if just switching from non-animated to animated */


float rotAngle;
glm::mat4 rotationMat;               /*  get computed every frame independent so no need to keep track */
glm::mat4 transformMat[NUM_PARTS];   /*  hierarchical transform, child is affected by parent */

glm::mat4 baseMVPMat, partMVPMat[NUM_PARTS];

/*  Matrices for view/projetion transformations */
glm::mat4 viewMat, projMat, vpMat;


/*  Color/depth of background */
const GLfloat bgColor[] = { 0.0f, 0.6f, 0.0f, 1.0f };
const GLfloat one = 1.0f;

/*  Non-color vectors, used to trigger texture and normal usage */
const glm::vec4 useTexture = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);
const glm::vec4 useNormal = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);

/*  Rendering mode - COLOR or NORMAL or WIREFRAME */
RenderMode currRenderMode = COLOR;

/*  Shader filenames */
const char vsFileName[]{ "shaders/shader.vert" };
const char fsFileName[]{ "shaders/shader.frag" };


/*  ID of the set of shaders that we'll use */
GLuint renderProg;

/*  Locations of the variables in the shader */
GLint textureLoc, colorLoc, mvpMatLoc;


/******************************************************************************/
/*!
\fn     void ValidateShader(GLuint shader, const char *file)
\brief
        Check whether shader files can be compiled successfully.
\param  shader
        ID of the shader
\param  file
        Shader file name
*/
/******************************************************************************/
void ValidateShader(GLuint shader, const char* file)
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];

    GLsizei length = 0;
    GLint result;

    glGetShaderInfoLog(shader, 512, &length, buffer);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE && length > 0)
    {
        std::cerr << "Shader " << file << " compilation error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Shader " << file << " compilation successful.\n";
}


/******************************************************************************/
/*!
\fn     void ValidateProgram(GLuint program)
\brief
        Check whether shader program can be linked successfully.
\param  program
        ID of the shader program
*/
/******************************************************************************/
void ValidateProgram(GLuint program)
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    GLint status;

    /*  Ask OpenGL to give us the log associated with the program */
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE && length > 0)
    {
        std::cerr << "Program " << program << " link error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Program " << program << " link successful.\n";

    /*  Ask OpenGL to validate the program */
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Error validating shader " << program << ".\n";
        exit(1);
    }
    else
    {
        std::cout << "Program " << program << " validation successful.\n";
    }
}


/******************************************************************************/
/*!
\fn     GLuint CompileShaders(char vsFilename[], char fsFilename[])
\brief
        Read the shader files, compile and link them into a program for render.
\param  const char vsFilename[]
        Vertex shader filename.
\param  const char fsFilename[]
        Fragment shader filename.
\return
        The rendering program ID.
*/
/******************************************************************************/
//GLuint CompileShaders(const char vsFilename[], const char fsFilename[])
//{
//    const char* vsSource = ReadTextFile(vsFilename);
//    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vsSource, NULL);
//    glCompileShader(vertexShader);
//    ValidateShader(vertexShader, vsFilename);           /*  Prints any errors */
//
//    const char* fsSource = ReadTextFile(fsFilename);
//    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fsSource, NULL);
//    glCompileShader(fragmentShader);
//    ValidateShader(vertexShader, fsFilename);           /*  Prints any errors */
//
//    const GLuint program = glCreateProgram();
//    glAttachShader(program, vertexShader);
//    glAttachShader(program, fragmentShader);
//    glLinkProgram(program);
//    ValidateProgram(program);                           /*  Print any errors */
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return program;
//}


/******************************************************************************/
/*!
\fn     void SendVertexData(Mesh &mesh)
\brief
        Create VAO (graphics state), VBO (vertex attribute state) and
        IBO (vertex index state) for the mesh, and feed vertex data into
        the shaders.
\param  mesh
        The input mesh, whose VAO/VBO/IBO members will be updated and data
        will be sent to shaders.
*/
/******************************************************************************/
void SendVertexData(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);

    glGenBuffers(1, &mesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER,
        mesh.numVertices * vertexSize, &mesh.vertexBuffer[0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        mesh.numIndices * indexSize, &mesh.indexBuffer[0],
        GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type,
            vLayout[i].normalized, vertexSize, (void*)&vLayout[i].offset);
    }
}



/******************************************************************************/
/*!
\fn     void SetUp()
\brief
        Set up the render program and graphics-related data for rendering.
*/
/******************************************************************************/
void SetUp()
{
    /*  Starting time */
    startTime = clock();
    animated = true;

    /*  Compile and link the shaders into rendering program */
   
    glUseProgram(renderProg);

    /*  Obtain the locations of the variables in the shaders with the given names */
    mvpMatLoc = glGetUniformLocation(renderProg, "mvpMat");
    colorLoc = glGetUniformLocation(renderProg, "color");
    textureLoc = glGetUniformLocation(renderProg, "tex");


    for (int i = 0; i < NUM_MESHES; ++i)
        SendVertexData(mesh[i]);

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*  Initially drawing using filled mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*  Hidden surface removal */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
}


/******************************************************************************/
/*!
\fn     void UpdateTransform(int partID)
\brief
        Update transformation/MVP matrices for part[partID]
\param  partID
        ID of the part we want to update
*/
/******************************************************************************/
void UpdateTransform(int partID)
{
    if (animated)
    {
        /*  Perform rotation if needed */
        if (fabs(part[partID].rotAmount) > EPSILON)
        {
            if (partID == TORSO)
            {
                /*  torso will rotate 360degs, but we keep the angle between 0 and 360.
                    For torso, rotAmount is rotation speed.
                */
                rotAngle = part[partID].rotAmount * secondsLapsed;
                while (rotAngle > TWO_PI)
                    rotAngle -= TWO_PI;
            }
            else
            {
                /*  Other parts, if rotated, maintain the angle between [-rotAmount, rotAmount].
                    We simply use sine of [(secondLapsed MOD PI) * 4] to rotate in this range.
                */
                int isecs = static_cast<int>(secondsLapsed / PI);
                float phase = 4.0f * (secondsLapsed - isecs * PI);
                rotAngle = part[partID].rotAmount * std::sinf(phase);
            }

            rotationMat = Rotate(rotAngle, part[partID].rotAxis);
        }
        else
            rotationMat = glm::mat4(1.0f);   /*  No rotation */


        /*  Torso will translate first, then rotate, to keep a distance from the world origin */
        if (partID == TORSO)
            transformMat[partID] = rotationMat * part[partID].tMat;
        else
            /*  Other parts will rotate, then be "shifted", in its parent's frame */
            transformMat[partID] = transformMat[parent[partID]] * part[partID].tMat * rotationMat;
    }

    /*  Update the final MVP matrix for this part,
        counting its own separate self-transformation that does not affect its children.
    */
    partMVPMat[partID] = vpMat * transformMat[partID] * part[partID].selfMat;
}


/******************************************************************************/
/*!
\fn     void UpdateUniforms_Draw(const Object &obj, const glm::mat4 &MVPMat)
\brief
        Update the color/texture, transformation matrix and vertex data of
        each object to shaders for rendering.
        Note that we do this per object, not per vertex.
\param  obj
        The object that we want to render.
\param  MVPMat
        The transformation matrix of the object. Object data is pre-defined as
        constants, so we have to store transform data separately.
*/
/******************************************************************************/
void UpdateUniforms_Draw(const Object& obj, const glm::mat4& MVPMat)
{
    if (currRenderMode == NORMAL)
        glUniform4fv(colorLoc, 1, glm::value_ptr(useNormal));
    /*  Trigger shader to use normal for color */
    else
        if (obj.imageID < 0 || currRenderMode == WIREFRAME)
            glUniform4fv(colorLoc, 1, glm::value_ptr(obj.color));
    /*  Use obj's color if drawing wireframes or objs that don't have textures */
        else
        {
            glUniform4fv(colorLoc, 1, glm::value_ptr(useTexture)); /* Trigger shader to use texture */
            glUniform1i(textureLoc, obj.imageID);           /*  Use obj's texture ID */
        }

    /*  Send MVP matrix to shaders */
    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(MVPMat));

    /*  Tell shader to use obj's VAO for rendering */
    glBindVertexArray(mesh[obj.meshID].VAO);
    glDrawElements(GL_TRIANGLES, mesh[obj.meshID].numIndices, GL_UNSIGNED_INT, nullptr);
}


/******************************************************************************/
/*!
\fn     void CleanUp()
\brief
        Clean up the graphics-related stuffs before shutting down.
*/
/******************************************************************************/
void CleanUp()
{
    glBindVertexArray(0);

    for (int i = 0; i < NUM_MESHES; ++i)
    {
        glDeleteVertexArrays(1, &mesh[i].VAO);
        glDeleteBuffers(1, &mesh[i].VBO);
        glDeleteBuffers(1, &mesh[i].IBO);
    }

    glDeleteProgram(renderProg);

    exit(0);    /*  successful run */
}


/******************************************************************************/
/*!
\fn     void Render()
\brief
        Render function for update & drawing.
*/
/******************************************************************************/
void Render()
{
    /*  Init background color/depth */
    glClearBufferfv(GL_COLOR, 0, bgColor);
    glClearBufferfv(GL_DEPTH, 0, &one);

    if (animated)
    {
        tempTime = clock();
        if (justAnimated)
        {
            idleTime += tempTime - currTime;
            justAnimated = false;
        }

        currTime = tempTime;
        /*  We subtract idleTime as well, to keep the animation smooth after the pause */
        secondsLapsed = 1.0f * (currTime - startTime - idleTime) / CLOCKS_PER_SEC;
    }


    /*  Send the floor data to shaders for rendering */
    UpdateUniforms_Draw(base, baseMVPMat);

    for (int i = 0; i < NUM_PARTS; ++i)
    {
        if (animated)
            UpdateTransform(i);

        /*  Send each part's data to shaders for rendering */
        UpdateUniforms_Draw(part[i], partMVPMat[i]);
    }

    /*  This is only needed for old computers that have only one framebuffer, or don't automatically
        swap the buffers. Not really showing siginificant effect on new computers.
    */
   // glutSwapBuffers();
}