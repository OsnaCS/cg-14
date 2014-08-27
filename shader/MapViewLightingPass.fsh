#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

// uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform float u_lightIntens;
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPos;

in VertexData {
	vec3 coordinates;
	vec2 uv;
} inData;

void main() {

	vec3 pos = inData.coordinates * texture(depthTexture, inData.uv).r + u_cameraPos;
	vec3 lightRay = pos - u_lightPosition;
	float dist = length(lightRay);
	float lightIntense = 3 * u_lightIntens / (0.05 + dist * dist);
	vec3 normal = texture(normalTexture, inData.uv).xyz;

	// --------------- Diffuse Light --------------------
	float diffuseIntensity;
	float cosa = (dot(-lightRay, ((normal * 2) - 1)) / (length(lightRay) * length(normal)));
	
	if(cosa < 0) {
		diffuseIntensity = 0;
	} 
	else {
		diffuseIntensity = lightIntense * abs(cosa); 
	}

	o_color = vec4(diffuseIntensity, diffuseIntensity, diffuseIntensity, 1);
}