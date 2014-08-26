#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData 
{
	vec2 uv;
	vec2 globalUV;
} inData;

uniform sampler2D s_menupng;
uniform sampler2D s_background;

// Gibt an wie stark geblurt werden soll, so größer die Zahl umso stärker
// Man kann sich an der Größe des Feldes orientieren, das geblurt werden soll
const float blurSize = 1.0/340.0;
const float blurSize2 = 1.0/180.0;

void main() 
{

	// Für Blur
	//vec4 save = texture(s_background, inData.globalUV);
	// Man hat einen ausgangs Vec4 mit Nullen, der dann Schrittweise
	// mit einem Prozentsatz der Werte der Umliegenden Pixel gefüllt wird
	// Die Werte werden aufsummiert, sodass das Bild verschwimmt
	vec4 save = vec4(0.0);

	// Für Textur
	//vec4 save = texture(s_savepng, inData.uv);

	// horizontaler teil des fragment shaders für Blur
	save += texture2D(s_background, vec2(inData.globalUV.x - 4.0*blurSize, inData.globalUV.y)) * 0.05;
  save += texture2D(s_background, vec2(inData.globalUV.x - 3.0*blurSize, inData.globalUV.y)) * 0.09;
  save += texture2D(s_background, vec2(inData.globalUV.x - 2.0*blurSize, inData.globalUV.y)) * 0.12;
  save += texture2D(s_background, vec2(inData.globalUV.x - blurSize, inData.globalUV.y)) * 0.15;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y)) * 0.16;
  save += texture2D(s_background, vec2(inData.globalUV.x + blurSize, inData.globalUV.y)) * 0.15;
  save += texture2D(s_background, vec2(inData.globalUV.x + 2.0*blurSize, inData.globalUV.y)) * 0.12;
  save += texture2D(s_background, vec2(inData.globalUV.x + 3.0*blurSize, inData.globalUV.y)) * 0.09;
  save += texture2D(s_background, vec2(inData.globalUV.x + 4.0*blurSize, inData.globalUV.y)) * 0.05;

  // vertikaler teil des fragment shaders für Blur
  // ließe sich evtl auch auslagern in eigenen Shader
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y - 4.0*blurSize2)) * 0.05;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y - 3.0*blurSize2)) * 0.09;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y - 2.0*blurSize2)) * 0.12;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y - blurSize2)) * 0.15;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y)) * 0.16;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y + blurSize2)) * 0.15;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y + 2.0*blurSize2)) * 0.12;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y + 3.0*blurSize2)) * 0.09;
  save += texture2D(s_background, vec2(inData.globalUV.x, inData.globalUV.y + 4.0*blurSize2)) * 0.05;

  if(texture2D(s_menupng, inData.uv).w > 0.1)
  {
  	save = texture(s_menupng, inData.uv);
	}

	
  // übernehmen der Farbe
	o_color = vec4(save.xyz, 1);

}