#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData 
{
	vec2 uv;
	vec2 globalUV;
} inData;

uniform sampler2D s_menupng;
uniform sampler2D s_resSh;
uniform int s_Sh;
uniform int s_number;

void main() 
{

	vec4 save = vec4(0.0);
	

	if((s_Sh == s_number) && texture(s_resSh, inData.uv).w > 0.1)
	{
		save = texture(s_resSh, inData.uv);
	}
	else if(texture(s_menupng, inData.uv).w > 0.1)
  {
  	save = texture(s_menupng, inData.uv);
	}
	else
	{
		discard;
	}

/*
  if(texture2D(s_menupng, inData.uv).w > 0.1)
  {
  	save = texture(s_menupng, inData.uv);
	}
	else 
	{
		discard;
	}
*/


  // übernehmen der Farbe
	o_color = vec4(save.xyz, 1);

}