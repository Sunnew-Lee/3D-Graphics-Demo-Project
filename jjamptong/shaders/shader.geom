#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

//uniform float sin;
//uniform mat3 rotate;
uniform float rotf;
uniform mat4 mvpMat;
in vec3 NRM[];
in vec4 teColor[];

out vec4 gColor;

vec4 Verts[3];

float random2d(vec2 coord){
    return fract(tan(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

mat3 rotate(float angle){
    return mat3(cos(angle),0,sin(angle),0,1,0, -sin(angle), 0, cos(angle));
}

void main(void) 
{
    gColor = teColor[0];
    gl_Position =mvpMat * gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = mvpMat *gl_in[0].gl_Position;
    EmitVertex();

    gl_Position =mvpMat * gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();

    gColor = vec4(0.56,0.8,0.56,1.0);
    Verts[0] = mvpMat *(gl_in[0].gl_Position+ gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
    //Verts[0].y += 5.0;
    //Verts[0].z += sin;wswsdaw
    gl_Position =( Verts[0] + vec4(0,5,0,0)) ;
    Verts[0].x = Verts[0].x + (random2d(vec2(cos(gl_in[0].gl_Position.y),sin(gl_in[0].gl_Position.x)))-0.5)*2 ;
    EmitVertex();

    gColor = teColor[0];
    gl_Position = Verts[0];
    gl_Position.x += random2d(vec2(cos(gl_in[0].gl_Position.x),sin(gl_in[0].gl_Position.x))) ;   
    EmitVertex();

    gl_Position = Verts[0];
    gl_Position.x -= random2d(vec2(cos(gl_in[0].gl_Position.x),sin(gl_in[0].gl_Position.x)));
    EmitVertex();

    // vec3 vc = gl_in[1].gl_Position.xyz -  gl_in[0].gl_Position.xyz;
    // gl_Position.xyz +=  rotate(30)  * vc;
    // EmitVertex();
    

    EndPrimitive();
}