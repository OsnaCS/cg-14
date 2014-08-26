#version 330 core

layout(location = 0) out vec3 o_normal;
//layout(location = 1) out vec3 o_depth;

in VertexData {

	vec2 uv;
	vec3 normal;
	//vec3 tangent;
//	float depth;

} inData;


void main() {
	
	/*
	vec3 normal = inData.normal;
	vec3 zA = normal; // normal goes in z-direction
	vec3 xA = inData.tangent; // tangent goes in x-direction
	vec3 yA = cross(zA, xA); // cross product results in the last axis (z-direction)

	mat3 texTrans = inverse(mat3(xA, yA, zA)); //matrix for coordinatesystem change

	normal = texture(texture1, inData.uv).xyz * texTrans;

	o_depth = vec3(0,0,0); //vec3(-inData.depth / u_backPlaneDistance, 1.0, 1.0);
	*/

	o_normal = inData.normal;
	

}
