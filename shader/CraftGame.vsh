#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec3 i_tangent;
layout(location = 3) in vec2 i_uv;


uniform mat4 u_projection;
uniform mat4 u_view;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 pos;
} outData;

void main() {

  mat4 pipeline = u_projection * u_view;

  outData.uv = i_uv;
  outData.normal = i_normal;
  outData.tangent = i_tangent;
  outData.pos = i_pos;

  // write gl_Position -> make OpenGL happy
  gl_Position = pipeline * vec4(i_pos, 1);
  // outData.pos=gl_Position.xyz;
}