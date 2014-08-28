#version 330 core



// we need to output a color
out vec4 o_color;

in VertexData {
	vec3 uvi;
	vec3 pos;
  float simpleLight;
} inData;

uniform ivec2 u_winSize;

uniform sampler2D s_lightTexture;
uniform sampler2DArray s_colorTexture;
uniform sampler2D s_depthTexture;

uniform float u_time;
uniform float u_daylength;

void main() {

	vec4 light = texture(s_lightTexture, gl_FragCoord.xy / u_winSize);
	vec4 depth = texture(s_depthTexture, gl_FragCoord.xy / u_winSize);
	vec4 texColor = texture(s_colorTexture, inData.uvi);
	
	texColor.xyz *= light.xyz;
  texColor.xyz *= inData.simpleLight;
	texColor.xyz *= light.w;

	vec3 fogColor = vec3(0.8,0.8,0.8);
	vec3 fogColor2 = vec3(0,0,0);

	// Modulo Berechnung von Zeit durch Tageslänge/4
  int m_time4 = int(u_time / (u_daylength/4));
  float modtime4 = u_time - (u_daylength/4) * m_time4;

  float day4 = u_daylength/4;
  float cvalue4 = modtime4/day4;

  // Modulo Berechnung von Zeit durch Tageslänge/8
  int m_time8 = int(u_time / (u_daylength/8));
  float modtime8 = u_time - (u_daylength/8) * m_time8;

  float day8 = u_daylength/8;
  float cvalue8 = modtime8/day8;

  float newlength = u_daylength;

  // Wenn Sonne zwischen Bereich 1 & 2 (Westen nach Oben)
  // bei steigender Sonne abnehmender Nebel
  if(m_time4==1)
  {
    newlength = (1 - cvalue4);
  }
  // Wenn Sonne zwischen Bereich 2 & 3 (Oben nach Osten)
  // bei sinkender Sonne zunehmender Nebel
  else if (m_time4==2)
  {
    newlength = cvalue4;
  }
  // Wenn Sonne zwischen Bereich 3 & 4 (Osten nach Unten)
  // bei sinkender Sonne abnehmender weißer Nebel und ab hälfte zunehmender Schwarzer
  else if (m_time4==3)
  {

  	if(m_time8 == 6)
  	{
    	newlength = (1-cvalue8);
    }
    else
    {
    	newlength = (-cvalue8)/2;
  	}

  }
  // Wenn Sonne zwischen Bereich 0 & 1 (Unten nach Westen)
  // bei steigender Sonne ab hälfte zunehmender weißer Nebel und abnehmender Schwarzer
  else
  {

  	if(m_time8 == 1)
  	{
    	newlength = cvalue8;
  	}
    else
    {
    	newlength = (-(1-cvalue8))/2;
    }

  }

  // Interpoliere die Farben
	texColor.xyz = mix(texColor.xyz, fogColor, depth.x * newlength);
  if(texColor.w <= 0.25){
    discard;
  }

	o_color = vec4(texColor.xyz, 1);
}