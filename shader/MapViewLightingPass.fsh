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
	vec3 normal = texture(normalTexture, inData.uv).xyz;
	normal = (normal * 2) - 1;
	
	vec3 lightRay = pos - u_lightPosition;
	float dist = length(lightRay);
	float lightIntense = 1 * u_lightIntens / (0.05 + ((dist * dist) * 0.01));

	// --------------- Diffuse Light --------------------
	float diffuseIntensity;
	float cosa = (dot(-lightRay, normal) / (length(lightRay) * length(normal)));
	
	if(cosa < 0) {
		diffuseIntensity = 0;
	} 
	else {
		diffuseIntensity = lightIntense * abs(cosa); 
	}

	// --------------- Specular Light --------------------
	float specularPower = 0.5;
	float specularExponent = 14;

	vec3 reflected = ((normal + lightRay) * dot(normal, -lightRay)) * 2;
	vec3 eyeRay = normalize(u_cameraPos - pos);				// from position to eye
	float cosRA = dot(reflected, eyeRay) / (length(reflected) * length(eyeRay));
	float specularIntensity = specularPower * u_lightIntens * pow(cosRA, specularExponent);

	o_color = vec4(diffuseIntensity, diffuseIntensity, diffuseIntensity, specularIntensity);
}