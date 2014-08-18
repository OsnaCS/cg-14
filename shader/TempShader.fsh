#version 330 core

// we need to output a color
layout(location = 0) out vec3 o_color;

in VertexData {
	vec2 uv;
} inData;

uniform sampler2D tex;

void main() {
	o_color = texture(tex, inData.uv*2).xyz;
}