#version 330 core



in VertexData
{

vec3 color;
vec2 texPos;

} inData;

uniform sampler2D tex;

void main()
{
        vec4 tempTex = texture2D(tex, inData.texPos);
        tempTex.w = 1-tempTex.y;
        tempTex.y = 0;
        tempTex.z = 0;
        gl_FragColor = tempTex;
}
