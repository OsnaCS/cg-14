#version 330 core

out vec4 o_color;

in VertexData 
{ 

vec3 color;
vec2 texPos;

} inData;

uniform sampler2D tex;

void main() 
{
	vec4 tempTex = texture(tex, inData.texPos);
	tempTex.w = 1-tempTex.y;
	tempTex.y = 0;
	tempTex.z = 0;
 	o_color = tempTex;
}
