#version 330 core

uniform vec4 color;
uniform sampler2D tex;
in vec2 UV;
in vec3 NRM;

out vec4 outColor;

void main(void)
{
    if (color.r < 0)
    {
        if (color.a < 0)
            outColor = vec4(texture(tex, UV).rgb, 1.0); /*  Use texture for color */
        else
            outColor = vec4(NRM.rgb, 1.0);              /*  Use normal for color */
    }
    else
        outColor = color;   /* Use obj's color as usual */
}