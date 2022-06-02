#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;

uniform mat4 mvpMat;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

out vec3 NRM;
out vec3 frag_pos;
//uniform vec3 lightPos;// = vec3(15.0, 0.0, 0.0);

//out vec3 u_light;

void main()
{
    
   // u_light = lightPos;
    // frag_pos = vec3(Model * vec4(pos, 1.0));
    // NRM = mat3(transpose(inverse(Model))) * nrm;
    // ShadowCoord = ShadowMatrix * Model * vec4(pos,1.0); 
    // gl_Position = mvpMat * vec4(pos, 1.0);
   
   NRM = normalize(NormalMatrix * nrm);
   frag_pos = (ModelViewMatrix * vec4(pos,1.0)).xyz;

   gl_Position = mvpMat * vec4(pos,1.0);

}
