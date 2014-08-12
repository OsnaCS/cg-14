#version 330 core

// we get the interpolated color from the vertex shader
in VertexData {
  vec3 color;
} inData;

// we need to output a color
out vec4 o_color;


void main() {
  
  vec3 color = inData.color;

  if(color.x < 0) color.x *= -1;
  if(color.y < 0) color.y *= -1;
  if(color.z < 0) color.z *= -1;


  o_color = vec4(color, 1);

}