#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
} inData;
uniform vec3 u_lightsource;

uniform sampler2D texture1;
uniform sampler2D normal_tex;

void main() {

	vec3 normal = inData.normal; // just a shorter name

	vec3 zA = normal; // copy of normal vector
	vec3 xA = inData.tangent; // x coordinate from the tangent
	vec3 yA = cross(zA, xA); // cross product results in the last coordinate

	mat3 texTrans = inverse(mat3(xA, yA, zA));

	normal = texture(normal_tex, inData.uv).xyz * texTrans;
	//normal =  vec3(0, 0, 1)* texTrans;



	// o_color = vec4(normal, 1);
	// return;


	//Veränderung der Normale
	
	float ambiente_intensity = 0.3;
	//vec3 light = vec3(-1, -3, 1);
	float diffuse_intensity;
	//float angle = (dot(light, normal)/(length(light)*length(normal))); 
	float angle = (dot(u_lightsource, normal)/(length(u_lightsource)*length(normal))); 
	
	if(angle>0){
		diffuse_intensity = 0;
	}else{
		diffuse_intensity = abs(angle); 
	}

	float total_intensity = diffuse_intensity+ambiente_intensity;
	total_intensity = clamp(total_intensity,0,1);
	vec4 texColor = texture(texture1, inData.uv);
	//vec4 texColor = vec4(normal,1);
	texColor = texColor*total_intensity;
	o_color = vec4(texColor.xyz, 1);
}