#version 330 core

// we need to output a color
out vec4 o_color;

in VertexData {
	vec2 uv;
	vec3 color;
} inData;

void main() {
  
	float epsilonMin = 0.01f;
	float epsilonMax = 1 - epsilonMin;

	if(inData.uv.x < epsilonMin || inData.uv.y < epsilonMin || inData.uv.x > epsilonMax || inData.uv.y > epsilonMax){

		o_color = vec4(0,0,0,1);

	} else {
		o_color = vec4(inData.color, 1);
	}
}