#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_world;

// forward color to fragment shader (will be linear interpolated)


void main() {

  mat4 pipeline = u_projection * u_view;
  mat4 final = pipeline * u_world;

  // write gl_Position -> make OpenGL happy
  gl_Position = final * vec4(i_pos, 1);
}