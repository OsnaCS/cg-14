#version 330 core

#define PI 3.1415926535897932384626433832795

uniform vec3 u_colorUp;
uniform vec3 u_colorWest;
uniform vec3 u_colorNorth;
uniform vec3 u_colorEast;
uniform vec3 u_colorSouth;
uniform vec3 u_colorDown;

out vec4 o_color;

in VertexData { vec3 coord; }
inData;

void main() {

  vec3 c1, c2, c3;
  float phi = 0;
	float theta = 0;
	float dpl;
	vec3 help = vec3(0,-1,0);
  dpl = dot(inData.coord, help) / (length(inData.coord) * length(help));
  dpl = clamp(dpl, -1, 1);
	theta = acos(dpl);
	theta /= (PI/2);

	help = vec3(1,0,0);
	inData.coord.y = 0;
	dpl = dot(inData.coord, help) / (length(inData.coord) * length(help));
	dpl = clamp(dpl, -1, 1);
	phi = acos(dpl);
	if(inData.coord.z< 0){
		phi = 2*PI - phi;
	}
	phi /=(PI/2);


	if(theta >1){

		theta--;
		c1 = u_colorUp;

	} else{

		c1 = u_colorDown;
		theta = 1 - theta;

	}

	if(phi == 4){
		phi = 0;
	}

	if(phi >= 3){

    phi -= 3;
    c2 = u_colorSouth;
		c3 = u_colorWest;

	}
	else if(phi >= 2){

		phi -= 2;
		c2 = u_colorEast;
		c3 = u_colorSouth;

	}
	else if(phi >= 1){

		phi -= 1;
		c2 = u_colorNorth;
		c3 = u_colorEast;

	}
	else if (phi >= 0){

		c2 = u_colorWest;
		c3 = u_colorNorth;
	}

  vec3 res = mix(c2, c3, phi);

  res = mix(res, c1, theta);
  o_color = vec4(res, 1);

 }
