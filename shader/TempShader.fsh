#version 330 core

// we need to output a color
layout(location = 0) out vec4 o_color;

in VertexData {
	vec2 uv;
} inData;

uniform sampler2D tex;

uniform bool u_pause;

// Gibt an wie stark geblurt werden soll, so größer die Zahl umso stärker
// Man kann sich an der Größe des Feldes orientieren, das geblurt werden soll
const float blurSize = 1.0/340.0;
const float blurSize2 = 1.0/180.0;

void main()
{

	if(u_pause)
	{

		// Für Blur
		// Man hat einen ausgangs Vec4 mit Nullen, der dann Schrittweise
		// mit einem Prozentsatz der Werte der Umliegenden Pixel gefüllt wird
		// Die Werte werden aufsummiert, sodass das Bild verschwimmt
		vec4 save = vec4(0.0);

		// horizontaler teil des fragment shaders für Blur
		save += texture(tex, vec2(inData.uv.x - 4.0*blurSize, inData.uv.y)) * 0.05;
	  save += texture(tex, vec2(inData.uv.x - 3.0*blurSize, inData.uv.y)) * 0.09;
	  save += texture(tex, vec2(inData.uv.x - 2.0*blurSize, inData.uv.y)) * 0.12;
	  save += texture(tex, vec2(inData.uv.x - blurSize, inData.uv.y)) * 0.15;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y)) * 0.16;
	  save += texture(tex, vec2(inData.uv.x + blurSize, inData.uv.y)) * 0.15;
	  save += texture(tex, vec2(inData.uv.x + 2.0*blurSize, inData.uv.y)) * 0.12;
	  save += texture(tex, vec2(inData.uv.x + 3.0*blurSize, inData.uv.y)) * 0.09;
	  save += texture(tex, vec2(inData.uv.x + 4.0*blurSize, inData.uv.y)) * 0.05;

	  // vertikaler teil des fragment shaders für Blur
	  // ließe sich evtl auch auslagern in eigenen Shader
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y - 4.0*blurSize2)) * 0.05;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y - 3.0*blurSize2)) * 0.09;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y - 2.0*blurSize2)) * 0.12;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y - blurSize2)) * 0.15;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y)) * 0.16;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y + blurSize2)) * 0.15;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y + 2.0*blurSize2)) * 0.12;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y + 3.0*blurSize2)) * 0.09;
	  save += texture(tex, vec2(inData.uv.x, inData.uv.y + 4.0*blurSize2)) * 0.05;

	  // Um das ganze Schwarzweiß zu machen
		float graur = save.x * 0.299;
		float graug = save.y * 0.587;
		float graub = save.z * 0.114;

		float grau = (graur+graug+graub);

		o_color = vec4(grau*0.5+0.05, grau*0.5+0.05, grau*0.5+0.05, 1);
	
	}
	else
	{

		o_color = vec4(texture(tex, inData.uv).xyz, 1);

	}

}