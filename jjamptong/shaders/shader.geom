#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform float sin;

in vec3 NRM[];
in vec4 vColor[];

out vec4 gColor;

vec4 Verts[3];

void main(void) 
{
    gColor = vColor[0];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();

    gColor = vec4(0.56,0.8,0.56,1.0);
    Verts[0] = (gl_in[0].gl_Position+ gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
    Verts[0].y += 2.0;
    //Verts[0].z += sin;
    gl_Position = Verts[0];
    EmitVertex();

    gColor = vColor[0];
    gl_Position = gl_in[0].gl_Position;
    gl_Position.x = gl_Position.x*0.75;    
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gl_Position.x = gl_Position.x*0.75;
    EmitVertex();

    EndPrimitive();
}