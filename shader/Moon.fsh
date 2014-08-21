#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 o_color;

uniform vec3 u_color;

in VertexData 
{ 

vec2 edge;

} inData;

void main() 
{


	if(inData.edge.y > 0){

		if(inData.edge.x * inData.edge.x + (inData.edge.y * inData.edge.y) * 2 <= 1){
	  	o_color = vec4(u_color, 1);
		} else{	
	  	o_color = vec4(0, 0, 0, 0);
		}
	}
	else{
		if(inData.edge.x * inData.edge.x + inData.edge.y * inData.edge.y <= 1){
	  	o_color = vec4(u_color, 1);
		} else{	
	  	o_color = vec4(0, 0, 0, 0);
		}
	}
}
