#version 330 core

layout(location = 0) in vec2 i_windowpos;
layout(location = 1) in vec2 i_texPos;


out VertexData
{
   vec2 texPos;
} outData;

void main()
{

        gl_Position = vec4(i_windowpos,0, 1);
        outData.texPos = i_texPos;
}
