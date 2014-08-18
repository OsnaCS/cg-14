#version 330 core

// we need to output a color
layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_position;

in VertexData {
	vec3 normal;
	vec3 position;
} inData;

void main() {
	o_normal = inData.normal;
	o_position = inData.position;
}