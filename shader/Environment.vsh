#version 330 core

layout(location = 0) in vec3 i_windowpos;
layout(location = 1) in vec3 i_color;

uniform float u_light;
uniform mat4 u_transform;

out VertexData{

	vec3 color;

	} outData;

void main(){

	gl_Position = u_transform * vec4(i_windowpos , 1);

	outData.color = i_color * u_light;

}