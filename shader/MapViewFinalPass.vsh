#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_uvi;
layout(location = 2) in float i_simpleLight;

uniform mat4 u_projection;
uniform mat4 u_view;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec3 uvi;
	vec3 pos;
	float simpleLight;
} outData;

void main() {

  outData.uvi = i_uvi;
  outData.pos = i_pos;
  outData.simpleLight = i_simpleLight;

  // write gl_Position -> make OpenGL happy
  gl_Position = u_projection * u_view * vec4(i_pos, 1);
}