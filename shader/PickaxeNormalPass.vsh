#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_transform;

out VertexData {
		vec2 uv;
		vec3 normal;
		float depth;
} outData;

void main()
{
	vec4 viewPosition = u_view * vec4(i_pos, 1);
  outData.depth = viewPosition.z;

	outData.uv = vec2(i_uv.x, 1-i_uv.y);

	outData.normal = (u_transform * vec4(i_normal, 1)).xyz;

  // write gl_Position -> make OpenGL happy
  gl_Position = u_projection * viewPosition;
}