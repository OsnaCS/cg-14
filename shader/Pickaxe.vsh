#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_uv;

uniform mat4 u_transform;

out VertexData
{
		vec2 uv;

} outData;


void main()
{
	outData.uv = vec2(i_uv.x, 1-i_uv.y);
	
	gl_Position = u_transform * vec4(i_pos,1);
}