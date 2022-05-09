#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 NRM[];
in vec4 vColor[];

out vec4 gColor;

vec4 Verts[3];
vec4 newColor;

void EmitTopVert(int i)
{
    Verts[i].z += 3.0f;
    gl_Position = (Verts[i]);
    gColor = newColor;
    EmitVertex();
}
void EmitVert(int i)
{
    gl_Position = (Verts[i]);
    gColor = vColor[i];
    EmitVertex();
}
void main(void) 
{
    //Verts[0] = gl_in[0].gl_Position;
    //Verts[1] = gl_in[1].gl_Position;
    //Verts[2] = (Verts[0]+Verts[1])/2;
    //newColor = vec4(1.0,1.0,1.0,1.0);

    //EmitVert(0);
    //EmitVert(1);
    //EmitTopVert(2);

    gl_Position = gl_in[0].gl_Position;
    gColor = vec4(0.0,0.4,0.0,0.1);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gColor = vec4(0.0,0.4,0.0,0.1);
    EmitVertex();

    gl_Position = (gl_in[0].gl_Position + gl_in[2].gl_Position)/2;
    gColor = vec4(1.0,1.0,1.0,1.0);
    EmitVertex();

    //gl_Position = gl_in[0].gl_Position;
    //gColor = vColor[i];
}