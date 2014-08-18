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
	o_color= vec4(0,1,0,1);

	float phi = 0;
	float theta = 0;
	float dpl;
	vec3 help = vec3(0,-1,0);
  dpl = dot(inData.coord, help) / (length(inData.coord) * length(help));
  dpl = clamp(dpl, -1, 1);
	theta = acos(dpl);
	theta /= PI;
	theta = theta * -1 + 1;

	help = vec3(1,0,0);
	inData.coord.y = 0;
	dpl = dot(inData.coord, help) / (length(inData.coord) * length(help));
	dpl = clamp(dpl, -1, 1);
	phi = acos(dpl);
	if(inData.coord.z< 0){
		phi = 2*PI - phi;
	}
	phi /=(2*PI);

	o_color = vec4(phi, theta, 0, 1);


/*	vec3 c1, c2, c3;
	vec3 planevec1;
	vec3 planevec2;
  float angle1;
  float angle2;
  float l;
  float dpl;

	if(inData.coord.y < 0){
		c1 = u_colorDown;
	}
	else{
		c1 = u_colorUp;
	}

	  // for u_colorWest,u_colorNorth
  if(inData.coord.x > 0 && inData.coord.z >= 0) {
  	c2 = u_colorWest;
  	c3 = u_colorNorth;

  	planevec2 = vec3(1,0,0);

  }
  // for u_colorNorth,u_colorEast
  else if(inData.coord.x <= 0 && inData.coord.z > 0) {
  	c2 = u_colorNorth;
  	c3 = u_colorEast;

  	planevec2 = vec3(0,0,1);
  }
  // for u_colorEast,u_colorSouth
  else if(inData.coord.x < 0 && inData.coord.z <= 0) {
  	c2 = u_colorEast;
  	c3 = u_colorSouth;

  	planevec2 = vec3(-1,0,0);
  }
  // for u_colorSouth,u_colorWest
  else if(inData.coord.x >= 0 && inData.coord.z < 0) {
  	c2 = u_colorSouth;
  	c3 = u_colorWest;

  	planevec2 = vec3(0,0,-1);
  }

  planevec1 = inData.coord;
  planevec1.y = 0;

  planevec2.y = 0;

  if(length(planevec1) == 0) {
    angle1 = 1;
  } else {


    l = length(inData.coord) * length(planevec1);
    dpl = dot(inData.coord, planevec1) / l;
    dpl = clamp(dpl, -1, 1);

    angle1 = acos(dpl);

    if(angle1 >= PI)
      angle1 = 2 * PI - angle1;

    angle1 /= (PI / 2);
    angle1 = clamp(angle1, 0, 1);
  }

  inData.coord.y = 0;
  l = length(inData.coord) * length(planevec2);
  dpl = dot(inData.coord, planevec2) / l;
  dpl = clamp(dpl, -1, 1);

  angle2 = acos(dpl);

  if(angle2 > PI)
    angle2 = 2 * PI - angle2;

  angle2 /= (PI / 2);
  angle2 = clamp(angle2,0,1);

  vec3 res;

  res = mix(c2, c3, angle2);

  res = mix(res, c1, angle1);
  o_color = vec4(res, 1);*/

 }
