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


/******************************************************************************/
/*  Graphics-related variables                                                */
/******************************************************************************/
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
bool animated = true;
bool justAnimated = false;  /*  To check if just switching from non-animated to animated */


float rotAngle;
Mat4 rotationMat;               /*  get computed every frame independent so no need to keep track */
Mat4 transformMat[NUM_PARTS];   /*  hierarchical transform, child is affected by parent */

Mat4 baseMVPMat, partMVPMat[NUM_PARTS];

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

/*  Rendering mode - COLOR or NORMAL or WIREFRAME */
RenderMode currRenderMode = COLOR;

/*  Shader filenames */


/*  ID of the set of shaders that we'll use */
GLuint renderProg;

/*  Locations of the variables in the shader */
GLint textureLoc, colorLoc, mvpMatLoc;

GLSLShader shdr_pgm;

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
\fn     void CompileShaders(char vsFilename[], char fsFilename[])
\brief
        Read the shader files, compile and link them into a program for render.
\param  const char vsFilename[]
        Vertex shader filename.
\param  const char fsFilename[]
        Fragment shader filename.
*/
/******************************************************************************/
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
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, 
                                vLayout[i].normalized, vertexSize, (void*)static_cast<__int64>(vLayout[i].offset));
    }
}


/******************************************************************************/
/*!
\fn     void ComputeViewProjMats()
\brief
        Set up the camera positions, orientations and view frustums.
*/
/******************************************************************************/
void ComputeViewProjMats()
{
    /*  Update view transform matrix */
    if (eyeMoved)
        viewMat = LookAtOrigin(1.0f * eyeRadius, ONE_STEP * eyeAlpha, ONE_STEP * eyeBeta);

    /*  Update projection matrix */
    if (resized)
        projMat = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

    /*  Update view/projection-related matrices for non-animating objects */
    if (eyeMoved || resized)
    {
        vpMat = projMat * viewMat;
        baseMVPMat = vpMat * base.selfMat;
    }
}


/******************************************************************************/
/*!
\fn     void SetUpTextures()
\brief
        Read texture images from files, then copy them to graphics memory.
*/
/******************************************************************************/
//void SetUpTextures()
//{
//    glGenTextures(NUM_IMAGES, textures);
//    unsigned char *imgData;
//    int imgWidth, imgHeight, numComponents;
//
//    for (int i = 0; i < NUM_IMAGES; ++i)
//    {
//        if (ReadImageFile(imgFileName[i], &imgData, &imgWidth, &imgHeight, &numComponents) == 0)
//        {
//            std::cerr << "Reading " << imgFileName[i] << " failed.\n";
//            exit(1);
//        }
//
//        /*  Activate texture memory GL_TEXTURE0 + i for texture image i */
//        glActiveTexture(GL_TEXTURE0 + i);
//
//        /*  Bind corresponding texture ID */
//        glBindTexture(GL_TEXTURE_2D, textures[i]);
//
//        /*  glEnable(GL_TEXTURE_2D) shouldn't be needed, but it seems that there's a bug in old ATI
//            drivers that require this for glGenerateMipmap to work. */
//        glEnable(GL_TEXTURE_2D);
//
//        /*  Handle image format's padding */
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
//
//        /*  Copy image data to graphics memory */
//        if (numComponents == 3)
//            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, 
//                            GL_RGB, GL_UNSIGNED_BYTE, imgData);
//        else
//            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, 
//                            GL_RGBA, GL_UNSIGNED_BYTE, imgData);
//
//        /*  Done with raw image data so delete it */
//        free(imgData);
//
//
//        /*  Generate texture mipmaps.
//            Probably won't need many levels of mipmaps,
//            since texture size is quite close to screen-space size */
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        /*  Set up texture behaviors */
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//    }
//}


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
    animated = true;

    /*  Compile and link the shaders into rendering program */
    CompileShaders();
    shdr_pgm.Use();

    /*  Obtain the locations of the variables in the shaders with the given names */
    mvpMatLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "mvpMat");
    colorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "color");
    textureLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "tex");

    ComputeViewProjMats();

    //SetUpTextures();

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

    shdr_pgm.UnUse();
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
void UpdateTransform(int partID, float delta_time)
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
                rotAngle = part[partID].rotAmount * delta_time;
                while (rotAngle > TWO_PI)
                    rotAngle -= TWO_PI;
            }
            else
            {
                /*  Other parts, if rotated, maintain the angle between [-rotAmount, rotAmount].
                    We simply use sine of [(secondLapsed MOD PI) * 4] to rotate in this range.
                */
                int isecs = static_cast<int>(delta_time / PI);
                float phase = 4.0f * (delta_time - isecs * PI);
                rotAngle = part[partID].rotAmount * std::sinf(phase);
            }

            rotationMat = Rotate(rotAngle, part[partID].rotAxis);
        }
        else
            rotationMat = Mat4(1.0f);   /*  No rotation */


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
\fn     void UpdateUniforms_Draw(const Object &obj, const Mat4 &MVPMat)
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
void UpdateUniforms_Draw(const Object &obj, const Mat4 &MVPMat)
{
    if (currRenderMode == NORMAL)
        glUniform4fv(colorLoc, 1, ValuePtr(useNormal)); 
        /*  Trigger shader to use normal for color */
    else
    if (obj.imageID < 0 || currRenderMode == WIREFRAME)
        glUniform4fv(colorLoc, 1, ValuePtr(obj.color)); 
        /*  Use obj's color if drawing wireframes or objs that don't have textures */
    else
    {
        glUniform4fv(colorLoc, 1, ValuePtr(useTexture)); /* Trigger shader to use texture */
        glUniform1i(textureLoc, obj.imageID);           /*  Use obj's texture ID */
    }

    /*  Send MVP matrix to shaders */
    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, ValuePtr(MVPMat));

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

    //glDeleteTextures(NUM_IMAGES, textures);

    glDeleteProgram(renderProg);

    exit(0);    /*  successful run */
}


/******************************************************************************/
/*!
\fn     void Resize(int w, int h)
\brief
        Update viewport and projection matrix upon window resize.
\param  w
        The new width of the window
\param  h
        The new height of the window
*/
/******************************************************************************/
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


/******************************************************************************/
/*!
\fn     void Render()
\brief
        Render function for update & drawing.
*/
/******************************************************************************/
void Render(double delta_time)
{
    /*  Init background color/depth */
    shdr_pgm.Use();
    glClearBufferfv(GL_COLOR, 0, bgColor);
    glClearBufferfv(GL_DEPTH, 0, &one);

    if (animated)
    {
        if (justAnimated)
        {
            justAnimated = false;
        }
    }

    ComputeViewProjMats();

    /*  Send the floor data to shaders for rendering */
    UpdateUniforms_Draw(base, baseMVPMat);

    for (int i = 0; i < NUM_PARTS; ++i)
    {
        if (animated || eyeMoved || resized)
            UpdateTransform(i, static_cast<float>(delta_time));

        /*  Send each part's data to shaders for rendering */
        UpdateUniforms_Draw(part[i], partMVPMat[i]);
    }

    /*  Reset */
    eyeMoved = false;
    resized = false;

    /*  This is only needed for old computers that have only one framebuffer, or don't automatically
        swap the buffers. Not really showing siginificant effect on new computers.
    */
    //glfwSwapBuffers(GLHelper::ptr_window);
    shdr_pgm.UnUse();
}