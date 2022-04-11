#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

uniform mat4 mvpMat;
uniform mat4 Model;

out vec2 UV;
out vec3 NRM;
out vec3 frag_pos;

void main(void) 
{
    gl_Position = mvpMat * vec4(pos, 1.0);
    frag_pos = vec3(Model * vec4(pos, 1.0));

    UV = uv;
    NRM = mat3(transpose(inverse(Model))) * nrm;
}