#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_texpos;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec2 globalUV;
} outData;

uniform float u_offset;

void main() {
	vec2 realPos = vec2(i_pos.x, i_pos.y + u_offset);

	outData.uv = i_texpos;
	outData.globalUV = (realPos + 1) / 2;

  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(realPos, 0, 1);

}