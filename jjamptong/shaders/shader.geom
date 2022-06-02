#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform float shrink ;
uniform float center ;
uniform float height ;
uniform vec3 uColor ;

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

    gColor = vec4(uColor,1.0);
    Verts[0] = (gl_in[0].gl_Position+ gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
    Verts[0].y += shrink * height;
    //Verts[0].z += sin;
    //gl_Position = Verts[0] +Verts[0]+ shrink * ( gl_in[2].gl_Position - Verts[0]);
    gl_Position = Verts[0];
    EmitVertex();


    gColor = vColor[0];
    gl_Position =  Verts[0] + shrink * (gl_in[0].gl_Position - Verts[0]);
    //gl_Position = gl_in[0].gl_Position;
    gl_Position.x = gl_Position.x * center;    
    EmitVertex();

    gl_Position = Verts[0] + shrink * (gl_in[1].gl_Position - Verts[0]);
    //gl_Position = gl_in[1].gl_Position;
    gl_Position.x = gl_Position.x * center;
    EmitVertex();

    EndPrimitive();
}
/*

void main(void) 
{

    gColor = vec4(0.56,0.8,0.56,1.0);
    Verts[0] = mvpMat *(gl_in[0].gl_Position+ gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
    //Verts[0].y += 5.0;
    //Verts[0].z += sin;wswsdaw
    gl_Position =( Verts[0] + vec4(0,5 + shrink,0,0)) ;
    Verts[0].x = Verts[0].x + (random2d(vec2(cos(gl_in[0].gl_Position.y),sin(gl_in[0].gl_Position.x)))-0.5)*2 ;
    EmitVertex();

    gColor = teColor[0];
    gl_Position = Verts[0];
    gl_Position.x += random2d(vec2(cos(gl_in[0].gl_Position.x),sin(gl_in[0].gl_Position.x))) * 0.25 ;   
    EmitVertex();

    gl_Position = Verts[0];
    gl_Position.x -= random2d(vec2(cos(gl_in[0].gl_Position.x),sin(gl_in[0].gl_Position.x))) * 0.25;
    EmitVertex();
    
    EmitVertex();
    

    EndPrimitive();
}
*/