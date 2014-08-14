#version 330 core

out vec4 o_color;

in VertexData
{

	vec3 color;

} inData;

void main()
{

	o_color = vec4(inData.color,1);

}
