#version 330 core



// we need to output a color
out vec4 o_color;

in VertexData {
  vec2 uv;
  vec3 pos;
  float depth;
//  float simpleLight;
} inData;

uniform ivec2 u_winSize;

uniform sampler2D s_lightTexture;
//uniform sampler2D s_colorTexture;
uniform sampler2D s_depthTexture;
uniform float u_backPlaneDistance;
//uniform float u_time;
//uniform float u_daylength;

void main() {

  vec4 light = texture(s_lightTexture, gl_FragCoord.xy / u_winSize);
  float depthFront = texture(s_depthTexture, gl_FragCoord.xy / u_winSize).x;
  float depthBack = -inData.depth / u_backPlaneDistance;

  //vec4 texColor = texture(s_colorTexture, inData.uv);
  
  //texColor.xyz *= light.xyz;
  //exColor.xyz *= inData.simpleLight;
  //texColor.xyz *= light.w;


  // Interpoliere die Farben
  //texColor.xyz = mix(texColor.xyz, fogColor, depth.x * newlength);
  //if(texColor.w <= 0.25){
  //  discard;
  //}

  o_color = vec4(1,1,1,(depthBack - depthFront)*u_backPlaneDistance/10);

}