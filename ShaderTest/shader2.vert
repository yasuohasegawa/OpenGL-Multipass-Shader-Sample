#version 120

// simple.vert

varying vec2 texcoord;

void main(void)
{
    texcoord = (gl_Position = gl_Vertex).xy * 0.5 + 0.5;
}