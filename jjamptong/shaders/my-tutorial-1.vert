#version 450 core

layout (location=8) in vec3 aVertexPosition;
layout (location=9) in vec3 aVertexColor;

out vec3 vInterpColor;

void main() {
  gl_Position = vec4(aVertexPosition, 1.0);
  vInterpColor = aVertexColor;
}
