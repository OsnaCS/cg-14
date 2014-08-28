#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 2) in vec2 i_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec3 u_position;
uniform mat4 u_transform;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec3 pos;
	float simpleLight;
} outData;

void main() {

  outData.uv = vec2(i_uv.x, 1 - i_uv.y);
  outData.pos = (u_transform * vec4(i_pos, 1)).xyz + u_position;
  outData.simpleLight = 1.0;

  // write gl_Position -> make OpenGL happy
  gl_Position = u_projection * u_view * vec4(outData.pos, 1);
}