#version 450 core

in vec3 vInterpColor;

out vec4 fFragColor;

void main () {
  fFragColor = vec4(vInterpColor, 1.0);
}

