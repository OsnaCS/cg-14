#version 330 core

layout(location = 0) in vec3 i_windowpos;

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
	// gl_Position.z = 254;

	outData.color = u_color;
}