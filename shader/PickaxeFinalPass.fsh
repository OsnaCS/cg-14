#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 pos;
} inData;

uniform ivec2 u_winSize;

uniform sampler2D s_lightTexture;
uniform sampler2D s_colorTexture;

void main() {

	vec4 light = texture(s_lightTexture, gl_FragCoord.xy / u_winSize);
	vec4 texColor = texture(s_colorTexture, inData.uv);
	
	texColor.xyz *= light.xyz;
	//texColor.xyz *= light.w;

	o_color = vec4(texColor.xyz, 1);
	// o_color = vec4(light.w);
}