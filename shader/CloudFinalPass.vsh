#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
//layout(location = 1) in vec3 i_normals;
layout(location = 2) in vec2 i_uv;
uniform mat4 u_projection;
uniform mat4 u_view;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec3 pos;
	float depth;
//	float simpleLight;
} outData;

void main() {

  outData.uv = i_uv;
  outData.pos = i_pos;
	vec4 viewPosition = u_view * vec4(i_pos+vec3(0,100,0), 1);

//	outData.uv = i_uv;
  outData.depth = viewPosition.z;
//  outData.simpleLight = i_simpleLight;

  // write gl_Position -> make OpenGL happy
  gl_Position = u_projection * u_view * vec4(i_pos + vec3(0,100,0), 1);
}