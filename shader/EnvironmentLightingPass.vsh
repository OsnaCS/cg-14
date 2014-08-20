#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_color;

uniform mat4 u_projection;
uniform mat4 u_view;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec3 position;
  vec3 color;
} outData;

void main() {
  outData.position = i_pos;
  outData.color = i_color;

  // write gl_Position -> make OpenGL happy
  gl_Position = (u_projection * u_view) * vec4(i_pos, 1);
  gl_Position.z -= 1;
}