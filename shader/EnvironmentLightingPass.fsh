#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

// uniform sampler2D depthTexture;
uniform sampler2D normalTexture;

uniform vec3 u_lightRay;
// uniform vec3 u_cameraPos;

in VertexData {
	vec3 coordinates;
	vec2 uv;
} inData;

void main() {
	// vec3 pos = inData.coordinates * texture(depthTexture, inData.uv).r + u_cameraPos;
	vec3 normal = texture(normalTexture, inData.uv).xyz;


	// --------------- Ambient Light --------------------
	float ambienteIntensity = 0.6;



	// --------------- Diffuse Light --------------------
	float diffuseIntensity;
	float angle = (dot(u_lightRay, normal)/(length(u_lightRay)*length(normal))); 
	
	if(angle > 0) {
		diffuseIntensity = 0;
	} 
	else {
		diffuseIntensity = abs(angle); 
	}



	// --------------- Specular Light --------------------
	float specularExponent = 8;
	float specularPower = 0.5;
	float specularIntensity = 1.0;

/*

	vec3 eyeRay = normalize(u_cameraPos - pos);				// from position to eye

	vec3 reflected = (2 * dot(normal, u_lightRay) * normal) - u_lightRay;

	//float specularAngle = cos(dot(eyeRay, reflected) / length(reflected)*length(eyeRay));
	// float specularAngle = cos(dot(normal, eyeRay));
	float specularAngle = cos((dot(reflected, eyeRay))/length(reflected));
	float specularIntensity = specularPower*pow(specularAngle,specularExponent);
	// float specularIntensity = specularAngle;

*/

	// --------------- Sum up light --------------------
	vec3 totalIntensity = vec3(ambienteIntensity + diffuseIntensity);
	o_color = vec4(totalIntensity, specularIntensity);
}