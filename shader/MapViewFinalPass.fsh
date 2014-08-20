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

	vec4 texColor = texture(s_colorTexture, inData.uv) * texture(s_lightTexture, gl_FragCoord.xy / u_winSize);
	// vec4 texColor = texture(s_lightTexture, (gl_FragCoord.xy + 1) );
	// vec3 texColor = texture(s_lightTexture, vec2(0,0)).xyz;
	o_color = vec4(texColor.xyz, 1);
	// o_color = vec4(gl_FragCoord.xy / u_winSize, 0, 1);
}