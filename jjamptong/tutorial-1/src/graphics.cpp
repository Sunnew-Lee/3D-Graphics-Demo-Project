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
#include <IG.h>


/*  Viewport width & height */
int width   = 1200;
int height  = 675;

/*  Camera view volume planes */
float nearPlane     = 1.0f;
float farPlane      = 800.0f;
float topPlane      = 0.6f * nearPlane;
float bottomPlane   = -topPlane;
float aspect        = 1.0f * width / height;
float rightPlane    = topPlane * aspect;
float leftPlane     = -rightPlane;


/*  For moving the camera */
int eyeAlpha    = 1;
int eyeBeta     = NUM_STEPS_PI / 2;
int eyeRadius   = EYE_MAX_RADIUS /2;
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

Vec3 lightPos = {2,15,0};

const Vec4 useNormal    = Vec4(-1.0f, -1.0f, -1.0f, 1.0f);

Mat4  shadowBias = Mat4(0);
Mat4 Lightview = Mat4(0);
Mat4 Lightproj = Mat4(0);
float resolutionPercen = 1.f;
/*  Shader filenames */

GLuint pass1Index, pass2Index;
/*  ID of the set of shaders that we'll use */
GLuint renderProg;

/*  Locations of the variables in the shader */
GLint colorLoc, mvpMatLoc, modelLoc, shadowLoc, lightLoc;

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


void CompileShaders()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;

    //////////////////////////////////////////////////////////////////////////////////////// For P_Modeling
    //shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/shader.vert"));
    //shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/shader.frag"));

    //////////////////////////////////////////////////////////////////////////////////////// For Toon_Shading
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/shadow.vert"));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/shadow.frag"));
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
    for (int i = 0; i < 2; ++i)
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
        wallMVPMat = vpMat * wall.selfMat;
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
    /*  Compile and link the shaders into rendering program */
    CompileShaders();
    shdr_pgm.Use();

    /*  Obtain the locations of the variables in the shaders with the given names */
    mvpMatLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "mvpMat");
    modelLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "Model");
    colorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "color");
    shadowLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "ShadowMatrix");
    lightLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "lightPos");

    pass1Index = glGetSubroutineIndex(shdr_pgm.GetHandle(), GL_FRAGMENT_SHADER, "recordDepth");
	pass2Index = glGetSubroutineIndex(shdr_pgm.GetHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");

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
void UpdateTransform(int partID)
{
    if (GLHelper::animated)
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

            //rotationMat = Rotate(rotAngle, part[partID].rotAxis);
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

void UpdateUniforms_Draw(const Object &obj, const Mat4 &MVPMat)
{

        glUniform4fv(colorLoc, 1, ValuePtr(obj.color));
        /*  Trigger shader to use normal for color */

    /*if (obj.imageID < 0 || GLHelper::currRenderMode == GLHelper::WIREFRAME)
        glUniform4fv(colorLoc, 1, ValuePtr(obj.color)); */
        /*  Use obj's color if drawing wireframes or objs that don't have textures */
    //else
    //{
    //    glUniform4fv(colorLoc, 1, ValuePtr(useTexture)); /* Trigger shader to use texture */
    //	glUniform1i(textureLoc, obj.imageID);           /*  Use obj's texture ID */
    //}
    /*  Send MVP matrix to shaders */
    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, ValuePtr(MVPMat));

    glUniform3fv(lightLoc, 1, ValuePtr(lightPos));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, ValuePtr(obj.selfMat));
    shadowBias = Mat4(Vec4(0.5f, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, 0.5f, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, 0.5f, 0.0f),
        Vec4(0.5f, 0.5f, 0.5f, 1.0f));
    Lightview = glm::lookAt(lightPos, { 0.,0.,0. }, { 0.,1.,0. });
    Lightproj = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	// = glm::perspective(glm::radians(45.f), (float)GLHelper::width / (float)GLHelper::height, nearPlane, farPlane);


    glUniformMatrix4fv(shadowLoc, 1, GL_FALSE, ValuePtr(shadowBias * Lightproj * Lightview));
   
    /*  Tell shader to use obj's VAO for rendering */
        
    glBindVertexArray(mesh[obj.meshID].VAO);
    glDrawElements(GL_TRIANGLES, mesh[obj.meshID].numIndices, GL_UNSIGNED_INT, nullptr);
}


void CleanUp()
{
    glBindVertexArray(0);
    
    for (int i = 0; i < NUM_MESHES-1; ++i)
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
    shdr_pgm.Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (GLHelper::animated)
    {
        tempTime = clock();
        if (GLHelper::justAnimated)
        {
            idleTime += tempTime - currTime;
            GLHelper::justAnimated = GL_FALSE;
        }

        currTime = tempTime;
        /*  We subtract idleTime as well, to keep the animation smooth after the pause */
        secondsLapsed = 1.0f * (currTime - startTime - idleTime) / CLOCKS_PER_SEC;
    }

    ImGui::SliderFloat3("light", &lightPos.x,-100.f,100.f);

    switch (static_cast<int>(GLHelper::currCameraMode))
    {
    case GLHelper::UP: MoveUp(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    case GLHelper::DOWN:MoveDown(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    case GLHelper::LEFT:MoveLeft(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    case GLHelper::RIGHT:MoveRight(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    case GLHelper::CLOSER:MoveCloser(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    case GLHelper::FARTHER:MoveFarther(); GLHelper::currCameraMode = GLHelper::IDLE; break;
    }

    GLfloat border[] = { 1.0f,0.0f,0.0f,0.0f };

    //The shadowmap texture 
    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width * resolutionPercen, height * resolutionPercen);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,GL_LESS);

    //Assign the shadow map to texture unit 0 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    //Create and set up the FBO 
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTex, 0);
    GLenum drawBuffers[] = { GL_NONE };
    glDrawBuffers(1, drawBuffers);

    //빛 입장에서 그려
    //텍스쳐에 들어감
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width * resolutionPercen,height * resolutionPercen);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glEnable(GL_FRONT_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    
    glPolygonOffset(2.5f,10.0f);
   // UpdateUniforms_Draw(wall, Lightproj * Lightview * wall.selfMat);
    //ImGui::SliderFloat("Resolution%", &resolutionPercen, 1.f, 4.f);
    ImGui::Text("Resolution"); ImGui::SameLine(85);
	if(ImGui::Button("Low"))
    {
        resolutionPercen = 1.f;
    }ImGui::SameLine(125);
    if(ImGui::Button("Medium"))
    {
        resolutionPercen = 2.f;
    }ImGui::SameLine(182);
    if (ImGui::Button("High"))
    {
        resolutionPercen = 3.f;
    }ImGui::SameLine(225);
    if (ImGui::Button("Very high"))
    {
        resolutionPercen = 4.f;
    }
    ComputeViewProjMats();
    UpdateUniforms_Draw(base, Lightproj * Lightview * base.selfMat);

    for (int i = 0; i < NUM_MESHES-1; ++i)
    {
        if (GLHelper::animated || eyeMoved || resized)
            partMVPMat[i] = Lightproj * Lightview * transformMat[i] * part[i].selfMat;

        /*  Send each part's data to shaders for rendering */
        UpdateUniforms_Draw(part[i], Lightproj * Lightview * part[i].selfMat);

    }

    // Revert to the default framebuffer for now
   // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFlush();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //카메라입장에서 그려
    //텍스쳐 세팅완료
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, GLHelper::width, GLHelper::height);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
    
    ComputeViewProjMats();

	//glUniformMatrix3fv(shadowLoc, 1, GL_FALSE, ValuePtr(vpMat));

    /*  Send the floor data to shaders for rendering */
   // UpdateUniforms_Draw(wall, wallMVPMat);
    UpdateUniforms_Draw(base, baseMVPMat);

    for (int i = 0; i < NUM_MESHES-1; ++i)
    {
        if (GLHelper::animated || eyeMoved || resized)
            UpdateTransform(i);

        /*  Send each part's data to shaders for rendering */
        UpdateUniforms_Draw(part[i], partMVPMat[i]);
        
    }

    /*  Reset */
    eyeMoved = false;
    resized = false;

    shdr_pgm.UnUse();
}