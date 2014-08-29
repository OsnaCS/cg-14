#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

uniform sampler2D depthTexture;
uniform sampler2D normalTexture;

uniform vec3 u_lightRay;
uniform vec3 u_cameraPos;
uniform float u_lightIntens;
uniform float u_ambient;

in VertexData {
	vec3 coordinates;
	vec2 uv;
} inData;

void main() {
	vec3 pos = inData.coordinates * texture(depthTexture, inData.uv).r + u_cameraPos;
	vec3 normal = texture(normalTexture, inData.uv).xyz;
	normal = (normal * 2) - 1;

	// --------------- Ambient Light --------------------
	float ambienteIntensity = u_ambient;

	// --------------- Diffuse Light --------------------
	float diffuseIntensity;
	float cosa = (dot(-u_lightRay, (normal))/(length(u_lightRay)*length(normal)));
	if(cosa < 0) {
		diffuseIntensity = 0;
	} else {
		diffuseIntensity = u_lightIntens * abs(cosa); 
	}

	// --------------- Specular Light --------------------
	float specularPower = 1;
	float specularExponent = 10;

	vec3 reflected = ((normal + u_lightRay) * dot(normal, -u_lightRay)) * 2;
	vec3 eyeRay = normalize(u_cameraPos - pos);				// from position to eye
	float cosRA = dot(reflected, eyeRay) / (length(reflected) * length(eyeRay));
	float specularIntensity = specularPower * u_lightIntens * pow(cosRA, specularExponent);

	// --------------- Sum up light --------------------
	vec3 totalIntensity = vec3(ambienteIntensity + diffuseIntensity);
	o_color = vec4(totalIntensity, specularIntensity);
}