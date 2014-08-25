#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 o_color;

uniform vec3 u_color;
uniform float u_phase;

in VertexData { vec2 edge; }
inData;

void main() {

  float help = 0;
  if(u_phase > 3) {
    if(inData.edge.x < 0) {
      help = u_phase - 3;
      if(inData.edge.y * inData.edge.y + (inData.edge.x * inData.edge.x) * (1 / help) <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    } else {
      if(inData.edge.y * inData.edge.y + inData.edge.x * inData.edge.x <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    }
  } else if(u_phase > 2) {
    if(inData.edge.x > 0) {
      help = u_phase - 2;
      help = 1 - help;
      if(inData.edge.y * inData.edge.y + (inData.edge.x * inData.edge.x) * (1 / help) >= 1
         && inData.edge.y * inData.edge.y + inData.edge.x * inData.edge.x <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    } else {
      o_color = vec4(0, 0, 0, 0);
    }
  } else if(u_phase > 1) {
    if(inData.edge.x < 0) {
      help = u_phase - 1;
      if(inData.edge.y * inData.edge.y + (inData.edge.x * inData.edge.x) * (1 / help) >= 1
         && inData.edge.y * inData.edge.y + inData.edge.x * inData.edge.x <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    } else {
      o_color = vec4(0, 0, 0, 0);
    }
  } else {
    help = u_phase;
    if(inData.edge.x > 0) {
      help = u_phase;
      help = 1 - help;
      if(inData.edge.y * inData.edge.y + (inData.edge.x * inData.edge.x) * (1 / help) <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    } else {
      if(inData.edge.y * inData.edge.y + inData.edge.x * inData.edge.x <= 1) {
        o_color = vec4(u_color, 1);
      } else {
        o_color = vec4(0, 0, 0, 0);
      }
    }
  }
}
