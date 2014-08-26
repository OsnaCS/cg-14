#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec3 i_coords;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
  vec3 coordinates;
  vec2 uv;
} outData;

void main() {
  outData.coordinates = i_coords;
  outData.uv = (i_pos + 1) / 2;

  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(i_pos, 0.999999999, 1);
}