#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

uniform mat4 mvpMat;
uniform mat4 Model;

out vec3 NRM;

//out vec3 v_normal;
out float visibility;
out vec3 frag_pos;

const float density=0.05;
const float gradient = 1.5;
void main()
{
    gl_Position = mvpMat * vec4(pos, 1.0);
    frag_pos = vec3(Model * vec4(pos, 1.0));

    NRM = mat3(transpose(inverse(Model))) * nrm;

    //v_normal=u_normalMatrix*nrm;
    
    float distance=length(gl_Position.xyz); // mvpMat * vec4(pos, 1.0);
    visibility=clamp(visibility,0.,1.);
}
