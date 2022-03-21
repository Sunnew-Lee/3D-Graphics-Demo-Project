#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

uniform mat4 mvpMat;

out vec3 NRM;

void main(void) 
{
    gl_Position = mvpMat * vec4(pos, 1.0);

    NRM = nrm;
}