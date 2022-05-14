/******************************************************************************/
/*!
\file   object.hpp
\par    Purpose: Definitions for scene objects
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   23/1/2015
*/
/******************************************************************************/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <imgui.h>

#include "math.hpp"
#include "mesh.hpp"

static ImVec4 Torus_color = ImVec4(0.1f, 0.1f, 0.9f, 1.00f);
static ImVec4 Torso_color = ImVec4(0.2f, 0.2f, 0.8f, 1.00f);
static ImVec4 Head_color = ImVec4(0.3f, 0.9f, 0.7f, 1.00f);
static ImVec4 Hat_color = ImVec4(0.4f, 0.4f, 0.6f, 1.00f);
//ImVec4 LArm_color = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
//ImVec4 LHand_color = ImVec4(0.6f, 0.6f, 0.4f, 1.00f);
//ImVec4 LLeg_color = ImVec4(0.7f, 0.7f, 0.3f, 1.00f);
//ImVec4 LFoot_color = ImVec4(0.8f, 0.8f, 0.2f, 1.00f);
//ImVec4 RArm_color = ImVec4(0.9f, 0.9f, 0.0f, 1.00f);
//ImVec4 RHand_color = ImVec4(1.0f, 0.0f, 0.1f, 1.00f);
//ImVec4 RLeg_color = ImVec4(0.9f, 0.0f, 0.5f, 1.00f);
//ImVec4 RFoot_color = ImVec4(0.8f, 0.0f, 0.3f, 1.00f);

struct Object
{
    int meshID;
    Vec4 color;
    Mat4 selfMat;       /*  self transformation that don't affect children, if any */
    Mat4 tMat;          /*  relative translation from parent, if any */
    Vec3 rotAxis;       /*  rotation about the parent, if any */
    float rotAmount;
    int imageID;        /*  ID for texture */

    Object( int meshID, Vec4 color, 
            Mat4 selfMat, Mat4 tMat = Mat4(1.0f)/*, Vec3 rotAxis = YAXIS, float rotAmount = 0.0f,
            int imageID = -1*/) :
        meshID(meshID), color(color), 
        selfMat(selfMat), tMat(tMat)/*, rotAxis(rotAxis), rotAmount(rotAmount),
        imageID(imageID)*/ { }
};


/*  The body parts of the character */
enum PartID                     
{ HEAD,  NUM_PARTS };

/*  The parent of each part */
const PartID parent[NUM_PARTS] 
{ HEAD};

/*  The ID for texture loading */
enum ImageID { BRICKS, FACE, JEANS, BALOONS, NUM_IMAGES };


const Object wall
{
    SPHERE,
    Vec4(Torus_color.x,Torus_color.y,Torus_color.z,Torus_color.w),                               /*  Color */
    Translate(0.0f, 9.4f, -10.0f)* Rotate(PI, XAXIS)
    * Scale(40.5f, 35.0f, 20.5f),                             /*  Self-transform */
   // Mat4(1.0f),                                                 /*  Translation w.r.t. parent */
   // YAXIS,                                                      /*  Rotation w.r.t. parent */
   // 0.0f
    /*  Texture image ID */
};
/*  The base on which the character is walking */
const Object base   
                {
                    SPHERE,
                    Vec4(Torus_color.x,Torus_color.y,Torus_color.z,Torus_color.w),                               /*  Color */
                    Translate(0.0f, -9.4f, 0.0f) 
                    *   Scale(20.5f, 16.0f, 20.5f),                             /*  Self-transform */
                  //  Mat4(1.0f),                                                 /*  Translation w.r.t. parent */
                  //  YAXIS,                                                      /*  Rotation w.r.t. parent */
                  //  0.0f
                                                                      /*  Texture image ID */
                };


const Object part[1]
{
	{
        /*  HEAD        */
        SPHERE,
        Vec4(Head_color.x,Head_color.y,Head_color.z,Head_color.w),                         /*  Color */
        Scale(5.f, 5.f, 5.f),                                /*  Self-transform */
      }
};

#endif
