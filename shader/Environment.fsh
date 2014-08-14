#version 330 core

out vec4 o_color;

in VertexData
{

	vec2 coord;

} inData;

void main()
{
	
	vec3 c1 = vec3(1,0,0);
	vec3 c2 = vec3(0,1,0);

	float x = inData.coord.x / 3.1415;

	vec3 res = x * c1 + (1 - x) * c2;
	o_color = vec4(res, 1);

}
