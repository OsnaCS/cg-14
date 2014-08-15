#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
} inData;

uniform sampler2D texture1;
uniform sampler2D normal_tex;

void main() {

	vec3 normal = inData.normal;

	vec3 yA = normal;
	vec3 xA = inData.tangent;
	vec3 zA = cross(yA, xA);

	mat3 texTrans = inverse(mat3(xA, yA, zA));

	normal = texture(normal_tex, inData.uv).xyz * texTrans;

	// o_color = vec4(normal, 1);
	// return;


	//Veränderung der Normale
	
	float ambiente_intensity = 0.3;
	vec3 light = vec3(-1, -3, 1);
	float diffuse_intensity;
	float angle = (dot(light, normal)/(length(light)*length(normal))); 
	
	if(angle>0){
		diffuse_intensity = 0;
	}else{
		diffuse_intensity = abs(angle); 
	}

	float total_intensity = diffuse_intensity+ambiente_intensity;
	total_intensity = clamp(total_intensity,0,1);
	// vec4 texColor = texture(texture1, inData.uv);
	vec4 texColor = vec4(0.6,0.6,0,1);
	texColor = texColor*total_intensity;
	o_color = vec4(texColor.xyz, 1);
}