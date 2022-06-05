#version 450 core

uniform sampler2D tex;

in vec4 gColor;
out vec4 outColor;

void main(void)
{
	outColor = gColor;   /* Use obj's color as usual */
}