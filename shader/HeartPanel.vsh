#version 330 core

layout(location = 0) in vec2 i_windowpos;
layout(location = 1) in vec3 i_color;
layout(location = 2) in vec2 i_texPos;

//uniform float u_light;
//uniform mat4 u_transform;

out VertexData
{

	vec3 color;
	vec2 texPos;
} outData;

void main()
{

	gl_Position = vec4(i_windowpos,0, 1);

	outData.color = i_color;
	outData.texPos = i_texPos;
}