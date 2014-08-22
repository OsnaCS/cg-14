#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData {
	vec2 uv;
} inData;

uniform sampler2D tex;
uniform bool u_pause;

void main() 
{

	float graur = texture(tex, inData.uv).x * 0.299;
	float graug = texture(tex, inData.uv).y * 0.587;
	float graub = texture(tex, inData.uv).z * 0.114;

	float grau = (graur+graug+graub);

	if(u_pause)
	{

		o_color = vec4(grau*0.5+0.05, grau*0.5+0.05, grau*0.5+0.05, 1);
	
	}
	else
	{

		o_color = vec4(texture(tex, inData.uv).xyz, 1);

	}

}