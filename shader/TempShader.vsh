#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec2 i_pos;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
} outData;

void main() {
	outData.uv = (i_pos + 1) / 2;

  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(i_pos, 0, 1);
}