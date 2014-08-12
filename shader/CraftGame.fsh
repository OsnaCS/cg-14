#version 330 core

uniform vec3 u_color;

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
} inData;

void main() {
  
	o_color = vec4(inData.uv.x, inData.uv.y, 0, 1);

  //o_color = vec4(u_color, 1);

}