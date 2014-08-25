#version 330 core

layout(location = 0) in vec3 i_windowpos;
layout(location = 1) in vec2 i_edge;

uniform float u_light;
uniform mat4 u_transform;

out VertexData
{

	vec2 edge;

} outData;

void main(){

	gl_Position = u_transform * vec4(i_windowpos.x, i_windowpos.y, i_windowpos.z, 1);

	outData.edge = i_edge;
}