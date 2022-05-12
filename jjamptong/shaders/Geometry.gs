#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 mvpMat;

uniform float uShrink = 0.9;

in vec4 vColor[];

out vec4 gColor;

vec3 Verts[3];
vec3 Center;

void EmitShrunkVertex(int i) {
    vec3 move_direction = Verts[i] - Center;
    gl_Position = mvpMat * vec4(Center+uShrink*move_direction,1);
    gColor = vColor[i];
    EmitVertex();
}


void main(){
    Verts[0] = gl_in[0].gl_Position.xyz;
    Verts[1] = gl_in[1].gl_Position.xyz;
    Verts[2] = gl_in[2].gl_Position.xyz;
    Center = (Verts[0] + Verts[1] + Verts[2])/3;
    EmitShrunkVertex(0);
    EmitShrunkVertex(1);
    EmitShrunkVertex(2);
   // EndPrimitive();
}