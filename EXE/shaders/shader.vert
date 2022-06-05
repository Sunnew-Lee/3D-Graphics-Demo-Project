#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;


uniform vec4 color;

out vec3 NRM;
out vec4 vColor;
out vec3 vPosition;

void main(void) 
{
 

    NRM = nrm;
    vColor = color;
    vPosition =   vec3(vec4(pos,1));
}