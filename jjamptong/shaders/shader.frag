#version 330 core

uniform vec3 u_light = vec3(30.0, 10.0, -40.0);
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);
uniform vec4 color;
uniform sampler2D tex;
in vec2 UV;
in vec3 NRM;

in vec3 frag_pos;
out vec4 outColor;

void main(void)
{
    vec3 n=normalize(NRM);
    vec3 l=normalize(u_light - frag_pos);
    float nl=max(dot(n,l),0.);//brightness
    vec3 diffuse = nl * u_lightColor;

    
    outColor = vec4(diffuse*color.xyz,1.0);   /* Use obj's color as usual */
}