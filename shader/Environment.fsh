#version 330 core

out vec4 o_color;

in VertexData
{

	vec3 coord;

} inData;

void main()
{

	vec3 c1 = vec3(1,0,0);
	vec3 c2 = vec3(0,1,0);

	inData.coord.y += 1;
	inData.coord.y /= 2;
	vec3 res = inData.coord.y * c1 + (1 - inData.coord.y) * c2;
	o_color = vec4(res, 1);

}
