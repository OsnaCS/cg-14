#version 330 core

uniform vec3 u_color;

// we need to output a color
out vec4 o_color;


void main() {
  
  o_color = vec4(u_color, 1);

}