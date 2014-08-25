#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData {
	vec2 uv;
	vec2 globalUV;
} inData;

uniform sampler2D s_save;

void main() 
{

	// Für Blur
	vec4 save = texture(s_save, inData.globalUV);

	// Für Textur
	//vec4 save = texture(s_save, inData.uv);

	o_color = vec4(save.xyz, 1);

}