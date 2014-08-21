#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData {
	vec2 uv;
} inData;

uniform sampler2D tex;

void main() {
	o_color = vec4(texture(tex, inData.uv).xyz, 1);
}