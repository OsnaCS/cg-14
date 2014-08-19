#version 330 core

layout(location = 0) in vec3 i_windowpos;
layout(location = 1) in vec3 i_color;

uniform float u_light;
uniform mat4 u_transform;
uniform vec3 u_color;

out VertexData
{

	vec3 color;

} outData;

void main()
{

	gl_Position = u_transform * vec4(i_windowpos.x, i_windowpos.y, i_windowpos.z, 1);

	if(i_windowpos.y > 0)
	{

		outData.color = i_color;

	} 
	else
	{
		outData.color = u_color;
	}
}