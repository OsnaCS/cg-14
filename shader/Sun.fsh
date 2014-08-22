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

	// float theta = 0;
	// float dpl;
	// vec2 help = vec2(0,1);
 //  dpl = dot(inData.edge, help) / (length(inData.edge) * length(help));
 //  dpl = clamp(dpl, -1, 1);
	// theta = acos(dpl);
	// theta /= PI;

	float distance = inData.edge.x * inData.edge.x + inData.edge.y * inData.edge.y;
 	float xh, yh, zh;
  if(distance <= 1- 0.08 * u_pulse) {
  	xh = (1 - u_color.x) * 0.6;
  	yh = (1 - u_color.y) * 0.6;
  	zh = (1 - u_color.z) * 0.6;
    o_color = vec4(u_color.x + xh*distance, u_color.y + yh*distance, u_color.z + zh*distance, 1);
  } else {
    o_color = vec4(u_color, 0.75 - (distance / 3));
  }
}
