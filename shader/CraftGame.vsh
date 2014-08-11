#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec3 i_color;

uniform mat4 u_trans;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
  vec3 color;
} outData;


void main() {
  // just forward position 
  outData.color = i_color;

  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(i_pos, 0, 1) * u_trans;
}