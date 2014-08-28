#version 330 core

layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_depth;

uniform float u_backPlaneDistance;

in VertexData {

	vec2 uv;
	vec3 normal;
  float depth;

} inData;


void main() {

	o_normal = (inData.normal+1)/2;
	o_depth = vec3(-inData.depth / u_backPlaneDistance, 1.0, 1.0);
	

}
