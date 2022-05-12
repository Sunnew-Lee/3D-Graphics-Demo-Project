#version 450 core
uniform mat4 GeometryTransform;

layout (location = 0) in vec3 pos;
uniform vec3 normal;

out vec4 vColor;

void main(){
    vColor = vec4(normal,1.0);
    gl_Position =  vec4(pos,1);
}
