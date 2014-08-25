#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

// uniform sampler2D depthTexture;
uniform sampler2D normalTexture;

uniform vec3 u_lightRay;
uniform float u_lightIntens;
uniform float u_daytime;
// uniform vec3 u_cameraPos;

in VertexData {
	vec3 coordinates;
	vec2 uv;
} inData;

void main() {
	// vec3 pos = inData.coordinates * texture(depthTexture, inData.uv).r + u_cameraPos;
	vec3 normal = texture(normalTexture, inData.uv).xyz;


	// --------------- Ambient Light --------------------

	float ambienteIntensity = u_daytime * u_daytime;
	if(ambienteIntensity > 0.5){
		ambienteIntensity = 1 - ambienteIntensity;
	}

	ambienteIntensity += 0.1;

	// --------------- Diffuse Light --------------------
	float diffuseIntensity;
	float cosa = (dot(-u_lightRay, ((normal*2)-1))/(length(u_lightRay)*length(normal)));
	// float cosa = (dot(vec3(-1,0,0), ((normal*2)-1))/(length(u_lightRay)*length(normal)));
	
	if(cosa < 0) {
		diffuseIntensity = 0;
	} 
	else {
		diffuseIntensity = u_lightIntens * abs(cosa); 
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