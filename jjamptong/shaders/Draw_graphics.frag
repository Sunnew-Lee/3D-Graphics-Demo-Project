#version 330 core

uniform vec4 color;
in vec3 NRM;

out vec4 outColor;

void main(void)
{
    if (color.r < 0)
    {
        outColor = vec4(NRM.rgb, 1.0);              /*  Use normal for color */
    }
    else
        outColor = color;   /* Use obj's color as usual */
}