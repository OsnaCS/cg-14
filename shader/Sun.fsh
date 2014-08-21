#version 330 core

uniform vec3 u_color;

out vec4 o_color;

in VertexData 
{ 

vec2 edge;

} inData;

void main(){

  if(inData.edge.x * inData.edge.x + inData.edge.y * inData.edge.y <= 1) {
    o_color = vec4(u_color, 1);
  } else {
    o_color = vec4(0, 0, 0, 0);
  }
}
