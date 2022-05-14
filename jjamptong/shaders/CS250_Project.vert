#version 330

//Dong-A Choi & Sunwoo Lee
//CS250 Class Project
//CS250
//2022 spring

layout (location=8) in vec3 aVertexPosition;
layout (location=9) in vec3 aVertexColor;

out vec3 vInterpColor;

void main() {
  gl_Position = vec4(aVertexPosition, 1.0);
  vInterpColor = aVertexColor;
}
