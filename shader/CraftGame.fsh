#version 330 core

// we get the interpolated color from the vertex shader
in VertexData {
  vec3 color;
} inData;

// we need to output a color
out vec4 o_color;


void main() {
  o_color = vec4(inData.color, 1);
}