#version 330 core

out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 normal;
	vec3 pos;
} inData;


uniform sampler2D texture1;


void main() {

	vec4 texColor = texture(texture1, inData.uv);
	o_color = vec4(texColor.xyz, 1);

}