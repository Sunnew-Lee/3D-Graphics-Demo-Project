#version 330

//Dong-A Choi & Sunwoo Lee
//CS250 Class Project
//CS250
//2022 spring
precision mediump float;

in vec3 vInterpColor;

out vec4 fFragColor;

void main () {

  fFragColor = vec4(vInterpColor, 1.0);


}

