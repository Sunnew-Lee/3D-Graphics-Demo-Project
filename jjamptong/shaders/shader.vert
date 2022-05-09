#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

uniform mat4 mvpMat;
uniform vec4 color;

out vec3 NRM;
out vec4 vColor;

void main(void) 
{
    gl_Position = mvpMat * vec4(pos, 1.0);

    NRM = nrm;
    vColor = color;
}