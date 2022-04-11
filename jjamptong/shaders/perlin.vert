#version 450 core

//Dong-A Choi & Sunwoo Lee
//CS250 Class Project
//CS250
//2022 spring

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec3 vVertexColor;
layout (location=2) in vec2 vVertexTexCoord;

layout (location=0) out vec3 vColor;
layout (location=1) out vec2 vTexCoord;

void main() {
  gl_Position = vec4(vVertexPosition, 0.0, 1.0);
  vColor = vVertexColor;
  vTexCoord = vVertexTexCoord;
}
