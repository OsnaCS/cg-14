#version 330 core

layout(location = 0) in vec2 i_marker;
layout(location = 1) in vec3 i_color;

out VertexData
{
        vec3 color;
} outData;

void main()
{
        gl_Position = vec4(i_marker,0, 1);
        outData.color = i_color;
}
