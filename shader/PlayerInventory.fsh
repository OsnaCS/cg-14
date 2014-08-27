#version 330 core

out vec4 o_color;

in VertexData
{
    vec2 texPos;
} inData;

uniform sampler2D tex;

void main()
{
        vec4 tempTex = texture(tex, inData.texPos);
        o_color = tempTex;
}
