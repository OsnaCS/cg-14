#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 pos;
} inData;
uniform vec3 u_lightRay;
uniform vec3 u_eyePoint;
uniform sampler2D texture1;
uniform sampler2D normal_tex;

void main() {

	vec3 normal = inData.normal; // just a shorter name

	vec3 zA = normal; // copy of normal vector
	vec3 xA = inData.tangent; // x coordinate from the tangent
	vec3 yA = cross(zA, xA); // cross product results in the last coordinate

	mat3 texTrans = inverse(mat3(xA, yA, zA));

	normal = texture(normal_tex, inData.uv).xyz * texTrans;



	//Light variables:
	float k_a = 0.25;
	float k_s = 0.1;
	float k_d	= 0.2;
	float o_d = 1.0;
	float o_s = 1.0;
	float o_e = 1.0;
	float i_e = 1.0;
	vec3 a = u_eyePoint-inData.pos;
	vec3 l = u_lightRay - inData.pos;
			 l = normalize(l);
	//R = 2(N \cdot L)N - L
	vec3 r = (2*(normal)*(dot(normal,l)))-l;
			 r = normalize(r);
	

	// Ambient light
	float ambiente_intensity = 0.6;

	// Diffuse light
	float diffuse_intensity;
	float angle = (dot(u_lightRay, normal)/(length(u_lightRay)*length(normal))); 
	
	if(angle>0){
		diffuse_intensity = 0;
	}else{
		diffuse_intensity = abs(angle); 
	}


	vec4 texColor = texture(texture1, inData.uv);

  // Specular light
	//Cs=ks*Ie*Os*cos(R,A)Oe;
	float angle_r_a = max(0,dot(r,a)/length(r)*length(a)); 
	float specular_intensity = k_s*i_e*texColor*pow(angle_r_a, o_e);


	// ----------------------------------------------
	float specularExponent = 8;
	float specularPower = 0.5;

	// vec3 lightRay = normalize(u_lightRay - inData.pos);	// from position to light
	vec3 lightRay = normalize(u_lightRay-inData.pos);
	vec3 eyeRay = normalize(u_eyePoint - inData.pos);				// from position to eye

	vec3 reflected = (2 * dot(normal, lightRay) * normal) - lightRay;

	//float specularAngle = cos(dot(eyeRay, reflected) / length(reflected)*length(eyeRay));
	float specularAngle = cos((dot(reflected,eyeRay))/length(reflected));
	float specularIntensity = specularPower*i_e*texColor*pow(specularAngle,specularExponent);




  // Total light
	float total_intensity = diffuse_intensity+ambiente_intensity+specularIntensity;
	//float total_intensity = diffuse_intensity+ambiente_intensity;
	total_intensity = clamp(total_intensity,0,1);
	texColor = texColor*total_intensity;
	o_color = vec4(texColor.xyz, 1);
	//vec3 outtmp = vec3(total_intensity);
	//o_color = vec4(outtmp,1);

}