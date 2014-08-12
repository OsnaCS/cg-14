#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_color;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_world;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
  vec3 color;
} outData;


void main() {
  // just forward position 
  outData.color = i_color;

  vec4 pos = vec4(i_pos, 1);

  pos = pos * u_world;

  mat4 pipe = u_projection * u_view;
  mat4 final = pipe * u_world;

  // pos /= pos.w;


  pos = u_view * pos;
  pos = u_projection * pos;

  // write gl_Position -> make OpenGL happy
  gl_Position = final  * vec4(i_pos, 1);
}