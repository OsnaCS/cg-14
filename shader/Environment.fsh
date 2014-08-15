#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 o_color;

in VertexData { vec3 coord; }
inData;

void main() {

  vec3 c0 = vec3(0, 0, 1);
  vec3 c1 = vec3(0.75, 0.75, 1);
  vec3 c2 = vec3(0.75, 0.75, 1);
  vec3 c3 = vec3(0.75, 0.75, 1);
  vec3 c4 = vec3(0.75, 0.75, 1);
  vec3 c5 = vec3(0, 0, 0);

  vec3 planevec = inData.coord;
  planevec.y = 0;

  float angle = acos(inData.coord * planevec / (length(inData.coord) * length(planevec)));

  if(angle > PI)
    angle = 2 * PI - angle;

  angle /= (PI / 2);

  vec3 res;
  if(inData.coord.y < 0)
    res = -inData.coord.y * angle * c5 + (1 + inData.coord.y * angle) * c2;
  else
    res = inData.coord.y * angle * c0 + (1 - inData.coord.y * angle) * c2;
  o_color = vec4(res, 1);

  if(angle>1||angle < 0)
  	o_color= vec4(1,0,0,1);
}
