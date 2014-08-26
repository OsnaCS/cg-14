#version 330 core

#define PI 3.1415926535897932384626433832795

uniform vec3 u_color;
uniform float u_pulse;

out vec4 o_color;

in VertexData 
{ 

vec2 edge;

} inData;

void main(){

	float distance = inData.edge.x * inData.edge.x + inData.edge.y * inData.edge.y;
 	float xh, yh, zh;
  if(distance <= 1- 0.04 * u_pulse) {
  	xh = (1 - u_color.x) * 0.6;
  	yh = (1 - u_color.y) * 0.4;
  	zh = (1 - u_color.z) * 0.2;
    o_color = (1 - 0.05 * u_pulse) * vec4(u_color.x + xh*distance, u_color.y + yh*distance, u_color.z + zh*distance, 1);
  } else {
    o_color = (1 - 0.1 * u_pulse) * vec4(u_color.x + distance / 18, u_color.y + distance / 36, u_color.z, 0.6 - (distance / 4));
  }
}
