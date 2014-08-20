#version 330 core

// we need to output a color
layout(location = 0) out vec3 o_color;

in VertexData {
	vec3 position;
	vec3 color;
} inData;

void main() {
	o_color = inData.color;
}