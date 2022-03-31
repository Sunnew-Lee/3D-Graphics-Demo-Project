#version 330 core

precision mediump float;

uniform vec3 u_light = vec3(5.0, 10.0, 0.0);
uniform vec3 fogColor = vec3(0.5, 0.5, 0.5);
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);
uniform float u_ambient = 0.5;
uniform vec4 color;

in vec2 UV;
in vec3 NRM;

in vec3 frag_pos;
in float visibility;
out vec4 outColor;

const float levels=4.;

void main(void)
{
    vec3 n=normalize(NRM);
    vec3 l=normalize(u_light - frag_pos);
    float nl=max(dot(n,l),0.);//brightness

    
    
    float level=floor(nl*levels);
    nl=level/levels;
    
    if(nl>=0&&nl<.25){nl=.25;}
    else if(nl>=.25&&nl<.5){nl=.5;}
    else if(nl>=.5&&nl<.75){nl=.75;}
    else if(nl>=.75&&nl<1){nl=1.;}

    vec3 diffuse = nl * u_lightColor;

    vec3 temp_color=(u_ambient+diffuse)*color.xyz;

    outColor=vec4(temp_color,1);

    outColor = mix(vec4(fogColor,1.),outColor,visibility);   /* Use obj's color as usual */
}