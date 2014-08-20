#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform vec3 u_cameraPos;

in VertexData {
	vec3 coordinates;
	vec2 uv;
} inData;

void main() {
	vec3 pos = inData.coordinates * texture(depthTexture, inData.uv).r + u_cameraPos;
	vec3 normal = texture(normalTexture, inData.uv).xyz;


	// --------------- Ambient Light --------------------
	float ambienteIntensity = 0.3;



	// --------------- Diffuse Light --------------------
	vec3 lightRay = vec3(1, -2, 1);
	float diffuseIntensity;
	float angle = (dot(lightRay, normal)/(length(lightRay)*length(normal))); 
	
	if(angle > 0) {
		diffuseIntensity = 0;
	} 
	else {
		diffuseIntensity = abs(angle); 
	}



	// --------------- Sum up light --------------------
	vec3 totalIntensity = vec3(ambienteIntensity + diffuseIntensity);
	o_color = vec4(totalIntensity, 0);
}