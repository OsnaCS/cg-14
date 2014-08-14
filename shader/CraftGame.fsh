#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 color;
} inData;

uniform sampler2D texture1;


void main() {

	o_color = vec4(texture(texture1, inData.uv).xyz, 1);







  
	float epsilonMin = 0.01f;
	float epsilonMax = 1 - epsilonMin;


}