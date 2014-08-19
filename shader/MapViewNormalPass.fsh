#version 330 core

// we need to output a color
layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_depth;

uniform float u_backPlaneDistance;

in VertexData {
	vec3 normal;
	float depth;
} inData;

void main() {

	o_depth = vec3(-inData.depth / u_backPlaneDistance, 1.0, 1.0);
	o_normal = inData.normal;
}