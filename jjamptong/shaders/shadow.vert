#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

out vec3 NRM;
out vec3 frag_pos;
out vec4 ShadowCoord; 

uniform mat4 mvpMat;
uniform mat4 Model;
uniform mat4 ShadowMatrix; 

uniform vec3 lightPos;// = vec3(15.0, 0.0, 0.0);

out vec3 u_light;

void main()
{
    // ShadowMatrix converts from modeling coordinates 
    // to shadow map coordinates. 
    u_light = lightPos;

    frag_pos = vec3(Model * vec4(pos, 1.0));
    NRM = mat3(transpose(inverse(Model))) * nrm;
    ShadowCoord = ShadowMatrix * Model * vec4(pos,1.0); 
    gl_Position = mvpMat * vec4(pos, 1.0);
   // NRM = mat3(vec3(Model[0]),vec3(Model[1]),vec3(Model[2])) * nrm;
    
    //float distance = length(gl_Position.xyz); // mvpMat * vec4(pos, 1.0);
}
