#version 330

//Dong-A Choi & Sunwoo Lee
//CS250 Class Project
//CS250
//2022 spring

// layout (location=8) in vec3 aVertexPosition;
// layout (location=9) in vec3 aVertexColor;

// uniform mat4 rotateMatrix;

// out vec3 vInterpColor;

// void main() {
//   //vec4 resultPosition = rotateMatrix * aVertexPosition;
//   gl_Position = aVertexPosition;
//   vInterpColor = aVertexColor;
// }

layout (location=8) in vec3 Position;
layout (location=9) in vec3 aVertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vInterpColor;

void main() {
  gl_Position = projection * view * model * vec4(Position, 1.0f);
  //gl_Position = vec4(Position,1.0f);
  vInterpColor = aVertexColor;
}
