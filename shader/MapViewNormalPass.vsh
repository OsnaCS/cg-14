#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec2 i_uv;

//  ??
layout(location = 3) in int i_tangentx;
//  ??
uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec3 u_normals[6];
uniform vec3 u_tangents[6];

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec3 normal;
	float depth;
	vec3 tangent;
} outData;

void main() {

  vec4 viewPosition = u_view * vec4(i_pos, 1);

	outData.uv = i_uv;
  outData.depth = viewPosition.z;
 // outData.normal = i_normal;
  outData.normal = u_normals[i_tangentx];
  outData.tangent = u_tangents[i_tangentx];

  // write gl_Position -> make OpenGL happy
  gl_Position = u_projection * viewPosition;
}