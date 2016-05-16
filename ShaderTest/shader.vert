#version 120

// simple.vert

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
