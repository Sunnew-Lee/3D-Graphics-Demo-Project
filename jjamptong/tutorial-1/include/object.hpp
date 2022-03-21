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

#include "math.hpp"
#include "mesh.hpp"


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
            Mat4 selfMat, Mat4 tMat = Mat4(1.0f), Vec3 rotAxis = YAXIS, float rotAmount = 0.0f,
            int imageID = -1) :
        meshID(meshID), color(color), 
        selfMat(selfMat), tMat(tMat), rotAxis(rotAxis), rotAmount(rotAmount),
        imageID(imageID) { }
};


/*  The body parts of the character */
enum PartID                     
{TORSO, HEAD,   HAT,    LARM,   RARM,   LHAND,  RHAND,  LLEG,   RLEG,   LFOOT,  RFOOT,  NUM_PARTS };

/*  The parent of each part */
const PartID parent[NUM_PARTS] 
{TORSO, TORSO,  HEAD,   TORSO,  TORSO,  LARM,   RARM,   TORSO,  TORSO,  LLEG,   RLEG};

/*  The ID for texture loading */
enum ImageID { BRICKS, FACE, JEANS, BALOONS, NUM_IMAGES };


/*  The base on which the character is walking */
const Object base   
                {
                    TORUS,
                    Vec4(0.2f, 0.1f, 0.1f, 1.0f),                               /*  Color */
                    Translate(0.0f, -9.4f, 0.0f) 
                    *   Scale(20.5f, 16.0f, 20.5f),                             /*  Self-transform */
                    Mat4(1.0f),                                                 /*  Translation w.r.t. parent */
                    YAXIS,                                                      /*  Rotation w.r.t. parent */
                    0.0f,
                    BRICKS                                                      /*  Texture image ID */
                };


const Object part[NUM_PARTS] 
                {
                    {   /*  TORSO       */
                        CUBE,
                        Vec4(0.5f, 0.5f, 1.0f, 1.0f),                           /*  Color */
                        Translate(0.0f, -0.1f, 0.0f) * Scale(2.5f, 4.2f, 1.3f), /*  Self-transform */
                        Translate(6.9f, 0.0f, 0.0f),                            /*  Translation w.r.t. parent */
                        YAXIS,                                                  /*  Rotation w.r.t. parent */
                        1.0f
                    },
                    {
                        /*  HEAD        */
                        SPHERE,
                        Vec4(1.0f, 0.83f, 0.16f, 1.0f),                         /*  Color */
                        Scale(1.5f, 1.5f, 1.5f),                                /*  Self-transform */
                        Translate(0.0f, 2.7f, 0.0f),                            /*  Translation w.r.t. parent */
                        YAXIS,                                                  /*  Rotation w.r.t. parent */
                        0.0f,
                        FACE                                                    /*  Texture image ID */
                    },
                    {   /*  HAT         */
                        CONE,
                        Vec4(0.55f, 0.85f, 0.95f, 1.0f),                        /*  Color */
                        Scale(1.6f, 1.5f, 1.6f) * Rotate(SIXTEENTH_PI, XAXIS),  /*  Self-transform */
                        Translate(0.0f, 1.1f, 0.2f),                            /*  Translation w.r.t. parent */
                        YAXIS,                                                  /*  Rotation w.r.t. parent */
                        0.0f,
                        BALOONS                                                 /*  Texture image ID */
                    },
                    {   /*  LEFT ARM    */
                        CYLINDER,
                        Vec4(1.0f, 0.0f, 0.0f, 1.0f),                           /*  Color */
                        Translate(0.0f, -2.0f, 0.0f) * Scale(0.7f, 4.0f, 0.7f), /*  Self-transform: translate off so that center of rotation is at shoulder */
                        Translate(1.6f, 1.8f, 0.0f),                            /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        EIGHTH_PI
                    },
                    {   /*  RIGHT ARM   */
                        CYLINDER,
                        Vec4(0.7f, 0.7f, 1.0f, 1.0f),                           /*  Color */
                        Translate(0.0f, -2.0f, 0.0f) * Scale(0.7f, 4.0f, 0.7f), /*  Self-transform: translate off so that center of rotation is at shoulder */
                        Translate(-1.6f, 1.8f, 0.0f),                           /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        -EIGHTH_PI
                    },
                    {   /*  LEFT HAND   */
                        PARTIAL_TORUS,
                        Vec4(1.0f, 0.83f, 0.16f, 1.0f),                         /*  Color */
                        Rotate(HALF_PI, XAXIS) * Scale(0.6f, 0.7f, 0.6f),       /*  Self-transform */
                        Translate(0.0f, -4.2f, 0.0f)                            /*  Translation w.r.t. parent */
                    },
                    {   /*  RIGHT HAND   */
                        PARTIAL_TORUS,
                        Vec4(1.0f, 0.83f, 0.16f, 1.0f),                         /*  Color */
                        Rotate(HALF_PI, XAXIS) * Scale(0.6f, 0.7f, 0.6f),       /*  Self-transform */
                        Translate(0.0f, -4.2f, 0.0f)                            /*  Translation w.r.t. parent */
                    },
                    {   /*  LEFT LEG    */
                        CYLINDER,
                        Vec4(1.0f, 0.0f, 0.0f, 1.0f),                        /*  Color */
                        Translate(0.0f, -2.5f, 0.0f) * Scale(1.2f, 5.0f, 1.2f), /*  Self-transform: translate off so that center of rotation is at hip */
                        Translate(-0.6f, -2.0f, 0.0f),                          /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        EIGHTH_PI
                        //JEANS                                                   /*  Texture image ID */
                    },
                    {   /*  RIGHT LEG    */
                        CYLINDER,
                        Vec4(0.19f, 0.22f, 0.26f, 1.0f),                        /*  Color */
                        Translate(0.0f, -2.5f, 0.0f) * Scale(1.2f, 5.0f, 1.2f) 
                        *   Rotate(PI, YAXIS),                                  /*  Self-transform: translate off so that center of rotation is at hip */
                        Translate(0.6f, -2.0f, 0.0f),                           /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        -EIGHTH_PI,
                        JEANS                                                   /*  Texture image ID */
                    },
                    {   /*  LEFT FOOT    */
                        CUBE,
                        Vec4(0.2f, 0.2f, 0.4f, 1.0f),                           /*  Color */
                        Translate(0.0f, 0.0f, -0.5f) * Scale(0.8f, 0.4f, 1.8f), /*  Self-transform: translate off so that center of rotation is at ankle */
                        Translate(0.0f, -5.0f, 0.0f),                           /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        -EIGHTH_PI
                    },
                    {   /*  RIGHT FOOT   */
                        CUBE,
                        Vec4(0.2f, 0.2f, 0.4f, 1.0f),                           /*  Color */
                        Translate(0.0f, 0.0f, -0.5f) * Scale(0.8f, 0.4f, 1.8f), /*  Self-transform: translate off so that center of rotation is at ankle */
                        Translate(0.0f, -5.0f, 0.0f),                           /*  Translation w.r.t. parent */
                        XAXIS,                                                  /*  Rotation w.r.t. parent */
                        EIGHTH_PI
                    },
                };

#endif
