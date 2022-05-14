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

#include "graphics.hpp"
//#include "textfile.h"
//#include "image_io.h"

#include <iostream>
#include <glslshader.h>
#include <glhelper.h>
#include <input.hpp>


/*  Viewport width & height */
int width   = 1200;
int height  = 675;

/*  Camera view volume planes */
float nearPlane     = 1.0f;
float farPlane      = 80.0f;
float topPlane      = 0.6f * nearPlane;
float bottomPlane   = -topPlane;
float aspect        = 1.0f * width / height;
float rightPlane    = topPlane * aspect;
float leftPlane     = -rightPlane;


/*  For moving the camera */
int eyeAlpha    = 1;
int eyeBeta     = NUM_STEPS_PI / 2;
int eyeRadius   = EYE_MAX_RADIUS;
bool eyeMoved   = true;     /*  to trigger view matrix update */
bool resized    = true;     /*  to trigger projection matrix update */


/*  For animating the objects */
clock_t startTime;      /*  The starting time of the program */
clock_t currTime;       /*  We'll use (currTime-startTime) to avoid accumulated imprecision */
clock_t idleTime = 0;   /*  Amount of time animation is paused, so we can keep the animation smooth */
clock_t tempTime;
float secondsLapsed;        /*  Num of seconds from startTime to currTime */


float rotAngle;
Mat4 rotationMat;               /*  get computed every frame independent so no need to keep track */
Mat4 transformMat[NUM_PARTS];   /*  hierarchical transform, child is affected by parent */

Mat4 baseMVPMat, wallMVPMat, partMVPMat[NUM_PARTS];

/*  Matrices for view/projetion transformations */
Mat4 viewMat, projMat, vpMat;


/*  Color/depth of background */
const GLfloat bgColor[] = { 0.0f, 0.6f, 0.0f, 1.0f };
const GLfloat one = 1.0f;

/*  Non-color vectors, used to trigger texture and normal usage */
const Vec4 useTexture   = Vec4(-1.0f, -1.0f, -1.0f, -1.0f);
const Vec4 useNormal    = Vec4(-1.0f, -1.0f, -1.0f, 1.0f);

/*  For texture loading/generation */
GLuint textures[NUM_IMAGES];
const char *imgFileName[NUM_IMAGES] 
{ "images/bricks.png", "images/face.png", "images/jeans.png", "images/baloons.png" };


/*  Shader filenames */


/*  ID of the set of shaders that we'll use */
GLuint renderProg;

/*  Locations of the variables in the shader */
GLint textureLoc, colorLoc, mvpMatLoc;

GLSLShader shdr_pgm;


void ValidateShader(GLuint shader, const char *file)
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


void CompileShaders()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/shader.vert"));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/shader.frag"));
    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void SendVertexData(Mesh &mesh)
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
    for (int i = 0; i < 1; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, 
                                vLayout[i].normalized, vertexSize, (void*)static_cast<__int64>(vLayout[i].offset));
    }
}

void ComputeViewProjMats()
{
    /*  Update view transform matrix */
        viewMat = LookAtOrigin(1.0f * eyeRadius, ONE_STEP * eyeAlpha, ONE_STEP * eyeBeta);

    /*  Update projection matrix */
        projMat = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
        vpMat = projMat * viewMat;
        baseMVPMat = vpMat * base.selfMat;
        wallMVPMat = vpMat * wall.selfMat;

}



void SetUp()
{
    /*  Starting time */
    /*  Compile and link the shaders into rendering program */
    CompileShaders();
    shdr_pgm.Use();

    /*  Obtain the locations of the variables in the shaders with the given names */
    mvpMatLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "mvpMat");
    colorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "color");
   // textureLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "tex");

    ComputeViewProjMats();

	SendVertexData(mesh[0]);

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


void CleanUp()
{
    glBindVertexArray(0);
    
    for (int i = 0; i < NUM_MESHES; ++i)
    {
        glDeleteVertexArrays(1, &mesh[i].VAO);
        glDeleteBuffers(1, &mesh[i].VBO);
        glDeleteBuffers(1, &mesh[i].IBO);
    }

    //glDeleteTextures(NUM_IMAGES, textures);

    glDeleteProgram(renderProg);

    exit(0);    /*  successful run */
}


void Resize(int w, int h)
{
    glViewport(0, 0, w, h);

    float changeW = 1.0f * w / width;
    float changeH = 1.0f * h / height;

    leftPlane *= changeW;
    rightPlane *= changeW;
    bottomPlane *= changeH;
    topPlane *= changeH;

    projMat = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

    width = w;
    height = h;

    /*  For triggering projection matrix update */
    resized = true;

    //glutPostRedisplay();    /*  Set flag to force re-rendering */
}

void Render()
{
    /*  Init background color/depth */
    shdr_pgm.Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearBufferfv(GL_COLOR, 0, bgColor);
    glClearBufferfv(GL_DEPTH, 0, &one);
  

    glUniform4fv(colorLoc, 1, ValuePtr(useNormal));

    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, ValuePtr(vpMat * part->selfMat));

    glBindVertexArray(mesh[part->meshID].VAO);
    glDrawElements(GL_TRIANGLES, mesh[part->meshID].numIndices, GL_UNSIGNED_INT, nullptr);
  

    shdr_pgm.UnUse();
}