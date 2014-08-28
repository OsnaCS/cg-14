#version 330 core

// we need to output a color
layout(location = 0) out vec3 o_normal;
layout(location = 1) out vec3 o_depth;

uniform float u_backPlaneDistance;
//uniform sampler2D normalTex;

in VertexData {
//	vec2 uv; (evtl spaeter)
	vec3 normal;
	float depth;
} inData;

void main() {

	vec3 normal = inData.normal; // just a shorter name

//	vec3 zA = normal; // copy of normal vector
//	vec3 xA = inData.tangent; // x coordinate from the tangent
//	vec3 yA = cross(zA, xA); // cross product results in the last coordinate

//	mat3 texTrans = inverse(mat3(xA, yA, zA));

	// Gibt ein Vec4 zurueck, wobei die 4. Komponente der Alpha-Wert ist
//	normal = texture(normalTex, inData.uv).xyz * texTrans;

	o_depth = vec3(-inData.depth / u_backPlaneDistance, 1.0, 1.0);
	o_normal = (normal + 1) / 2;
}