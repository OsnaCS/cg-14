#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 normal;
} inData;

uniform sampler2D texture1;

void main() {
	
	float ambiente_intensity = 0.3;

	vec3 light = vec3(-1, -3, 1);
	float diffuse_intensity;
	float angle = (dot(light, inData.normal)/(length(light)*length(inData.normal))); 
	
	if(angle>0){
		diffuse_intensity = 0;
	}else{
		diffuse_intensity = abs(angle); 
	}

	float total_intensity = diffuse_intensity+ambiente_intensity;
	total_intensity = clamp(total_intensity,0,1);
	vec4 texColor = texture(texture1, inData.uv);
	texColor = texColor*total_intensity;
	o_color = vec4(texColor.xyz, 1);
}