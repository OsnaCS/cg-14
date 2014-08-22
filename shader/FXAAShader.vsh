#version 330 core

layout(location=0) in vec2 i_position;

out VertexData {
	vec2 uv;
} outData;

void main()
{
  gl_Position =vec4(i_position, 0, 1.0);
  
  // übergebene Koordinaten umrechnen: (-1,-1)->(0,0), (1,1)->(1,1), (-1,1)->(0,1), (1,-1)->(1,0)
  outData.uv =(i_position + vec2(1,1)) * vec2(0.5,0.5);
}