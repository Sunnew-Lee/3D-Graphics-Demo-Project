#version 450 core

//Dong-A Choi & Sunwoo Lee
//CS250 Class Project
//CS250
//2022 spring

layout (location=0) in vec3 vColor;
layout (location=1) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTex2d;

void main () {
    fFragColor = texture(uTex2d,vTexCoord);
}

