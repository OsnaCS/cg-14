#include "Environment.hpp"

#include <math.h>

Environment::Environment() : m_dayLength(10), m_time(0) {}

void Environment::draw(Mat4f viewMat, Mat4f projMat)
{

	// Vec3f pos = getSunPos();
	// slog("time: ", m_time);
	// slog("pos: ", pos);

	m_program.prime([&](HotProgram& hotprog)
	{

		viewMat.setColumn(3, Vec4f(0,0,0,1));

		hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.uniform["u_colorUp"] = getcUp();
		hotprog.uniform["u_colorWest"] = getcWest();
		hotprog.uniform["u_colorNorth"] = getcNorth();
		hotprog.uniform["u_colorEast"] = getcEast();
		hotprog.uniform["u_colorSouth"] = getcSouth();
		hotprog.uniform["u_colorDown"] = getcDown();

		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);

	});

  m_program2.prime([&](HotProgram& hotprog)
  {

    float a = m_time * 3.1415 * 2 / (m_dayLength);

    hotprog.uniform["u_sunColor"] = getSunColor();
    hotprog.uniform["u_moonColor"] = Vec3f(0.75, 0.75, 0.75);

    viewMat.setColumn(3, Vec4f(0,0,0,1));

    Mat4f rotMat = rotationMatrix(quaternionFromAxisAngle(Vec3f(0,0,1), a));

    hotprog.uniform["u_transform"] = projMat * (viewMat * rotMat);
    hotprog.draw(m_sun, PrimitiveType::TriangleStrip);

  });

}

void Environment::init()
{

	VShader vs;
	vs.compile(loadShaderFromFile("shader/Environment.vsh"));
	FShader fs;
	fs.compile(loadShaderFromFile("shader/Environment.fsh"));

	constexpr float PI = 3.1415926;

  // create program and link the two shaders
	m_program.create(vs, fs);

  m_sphere.create(3 + 2, 6, 13);
  m_sphere.prime<Vec3f, Vec2f>([](HotVertexSeq<Vec3f, Vec2f>& hot)
  {

  	hot.vertex[0].set(Vec3f(0, 1, 0),  Vec2f(0,0));
  	hot.vertex[1].set(Vec3f(1, 0, 0),  Vec2f(PI/2,0));
  	hot.vertex[2].set(Vec3f(0, 0, 1) , Vec2f(PI/2,0));
  	hot.vertex[3].set(Vec3f(-1, 0, 0), Vec2f(PI/2,0));
  	hot.vertex[4].set(Vec3f(0, 0, -1), Vec2f(PI/2,0));
  	hot.vertex[5].set(Vec3f(0, -1, 0), Vec2f(PI,0));

  	hot.index[0] = 0;
  	hot.index[1] = 2;
  	hot.index[2] = 1;
  	hot.index[3] = 5;
  	hot.index[4] = 4;
    hot.index[5] = 3;
    hot.index[6] = GLIndex::PrimitiveRestart;
  	hot.index[7] = 5;
  	hot.index[8] = 2;
  	hot.index[9] = 3;
  	hot.index[10] = 0;
  	hot.index[11] = 4;
  	hot.index[12] = 1;

  });

  VShader vs2;
  vs2.compile(loadShaderFromFile("shader/Sun.vsh"));
  FShader fs2;
  fs2.compile(loadShaderFromFile("shader/Sun.fsh"));

  // create program and link the two shaders
  m_program2.create(vs2, fs2);

  m_sun.create(3, 34, 67);
  m_sun.prime<Vec3f>([](HotVertexSeq<Vec3f>& hot)
  {
  	// Sonne
  	// 4 Ecken
    hot.vertex[0].set(Vec3f( 0, -20, 1));
    hot.vertex[1].set(Vec3f(-1, -20, 0));
    hot.vertex[2].set(Vec3f( 1, -20, 0));
    hot.vertex[3].set(Vec3f( 0, -20, -1));

    // Mittelpunkt
    hot.vertex[4].set(Vec3f(0, -20, 0));

    // 12 Restecken
    hot.vertex[5].set(Vec3f(  sin(3.1415/8)  , -20, cos(3.1415/8)));
    hot.vertex[6].set(Vec3f(  sin(2*3.1415/8), -20, cos(2*3.1415/8)));
    hot.vertex[7].set(Vec3f(  sin(3*3.1415/8), -20, cos(3*3.1415/8)));
    hot.vertex[8].set(Vec3f(  cos(3.1415/8)  , -20, -sin(3.1415/8)));
    hot.vertex[9].set(Vec3f(  cos(2*3.1415/8), -20, -sin(2*3.1415/8)));
    hot.vertex[10].set(Vec3f( cos(3*3.1415/8), -20, -sin(3*3.1415/8)));
    hot.vertex[11].set(Vec3f(-sin(3.1415/8)  , -20, -cos(3.1415/8)));
    hot.vertex[12].set(Vec3f(-sin(2*3.1415/8), -20, -cos(2*3.1415/8)));
    hot.vertex[13].set(Vec3f(-sin(3*3.1415/8), -20, -cos(3*3.1415/8)));
    hot.vertex[14].set(Vec3f(-cos(3.1415/8)  , -20, sin(3.1415/8)));
    hot.vertex[15].set(Vec3f(-cos(2*3.1415/8), -20, sin(2*3.1415/8)));
    hot.vertex[16].set(Vec3f(-cos(3*3.1415/8), -20, sin(3*3.1415/8)));

    // Mond
  	// 4 Ecken
    hot.vertex[17].set(Vec3f( 0, 20, 1) );
    hot.vertex[18].set(Vec3f(-1, 20, 0) );
    hot.vertex[19].set(Vec3f( 1, 20, 0) );
    hot.vertex[20].set(Vec3f( 0, 20, -1));

    // Mittelpunkt
    hot.vertex[21].set(Vec3f(0, 20, 0));

    // 12 Restecken
    hot.vertex[5  + 17].set(Vec3f( sin(3.1415/8)  , 20, cos(3.1415/8))   );
    hot.vertex[6  + 17].set(Vec3f( sin(2*3.1415/8), 20, cos(2*3.1415/8)) );
    hot.vertex[7  + 17].set(Vec3f( sin(3*3.1415/8), 20, cos(3*3.1415/8)) );
    hot.vertex[8  + 17].set(Vec3f( cos(3.1415/8)  , 20, -sin(3.1415/8))  );
    hot.vertex[9  + 17].set(Vec3f( cos(2*3.1415/8), 20, -sin(2*3.1415/8)));
    hot.vertex[10 + 17].set(Vec3f( cos(3*3.1415/8), 20, -sin(3*3.1415/8)));
    hot.vertex[11 + 17].set(Vec3f(-sin(3.1415/8)  , 20, -cos(3.1415/8))  );
    hot.vertex[12 + 17].set(Vec3f(-sin(2*3.1415/8), 20, -cos(2*3.1415/8)));
    hot.vertex[13 + 17].set(Vec3f(-sin(3*3.1415/8), 20, -cos(3*3.1415/8)));
    hot.vertex[14 + 17].set(Vec3f(-cos(3.1415/8)  , 20, sin(3.1415/8))   );
    hot.vertex[15 + 17].set(Vec3f(-cos(2*3.1415/8), 20, sin(2*3.1415/8)) );
    hot.vertex[16 + 17].set(Vec3f(-cos(3*3.1415/8), 20, sin(3*3.1415/8)) );


    hot.index[0] = 0;
    hot.index[1] = 5;
    hot.index[2] = 4;
    hot.index[3] = 6;
    hot.index[4] = 7;
    hot.index[5] = GLIndex::PrimitiveRestart;
    hot.index[6] = 7;
    hot.index[7] = 2;
    hot.index[8] = 4;
    hot.index[9] = 8;
    hot.index[10] = 9;
    hot.index[11] = GLIndex::PrimitiveRestart;
    hot.index[12] = 9;
    hot.index[13] = 10;
    hot.index[14] = 4;
    hot.index[15] = 3;
    hot.index[16] = 11;
    hot.index[17] = GLIndex::PrimitiveRestart;
    hot.index[18] = 11;
    hot.index[19] = 12;
    hot.index[20] = 4;
    hot.index[21] = 13;
    hot.index[22] = 1;
	  hot.index[23] = GLIndex::PrimitiveRestart;
	  hot.index[24] = 1;
	  hot.index[25] = 14;
	  hot.index[26] = 4;
	  hot.index[27] = 15;
	  hot.index[28] = 16;
	  hot.index[29] = GLIndex::PrimitiveRestart;
    hot.index[30] = 16;
    hot.index[31] = 0;
    hot.index[32] = 4;

	  hot.index[33] = GLIndex::PrimitiveRestart;

    hot.index[0+34]  =  0 + 17;
    hot.index[1+34]  =  5 + 17;
    hot.index[2+34]  =  4 + 17;
    hot.index[3+34]  =  6 + 17;
    hot.index[4+34]  =  7 + 17;
    hot.index[5+34]  =  GLIndex::PrimitiveRestart;
    hot.index[6+34]  =  7 + 17;
    hot.index[7+34]  =  2 + 17;
    hot.index[8+34]  =  4 + 17;
    hot.index[9+34]  =  8 + 17;
    hot.index[10+34] =  9 + 17;
    hot.index[11+34] =  GLIndex::PrimitiveRestart;
    hot.index[12+34] =  9 + 17;
    hot.index[13+34] = 10 + 17;
    hot.index[14+34] =  4 + 17;
    hot.index[15+34] =  3 + 17;
    hot.index[16+34] = 11 + 17;
    hot.index[17+34] = GLIndex::PrimitiveRestart;
    hot.index[18+34] = 11 + 17;
    hot.index[19+34] = 12 + 17;
    hot.index[20+34] =  4 + 17;
    hot.index[21+34] = 13 + 17;
    hot.index[22+34] =  1 + 17;
	  hot.index[23+34] = GLIndex::PrimitiveRestart;
	  hot.index[24+34] =  1 + 17;
	  hot.index[25+34] = 14 + 17;
	  hot.index[26+34] =  4 + 17;
	  hot.index[27+34] = 15 + 17;
	  hot.index[28+34] = 16 + 17;
	  hot.index[29+34] = GLIndex::PrimitiveRestart;
    hot.index[30+34] = 16 + 17;
    hot.index[31+34] =  0 + 17;
    hot.index[32+34] =  4 + 17;

  });

}

void Environment::update(float delta)
{

	m_time += delta;

	if(m_time > m_dayLength)
	{
		m_time -= m_dayLength;
	}

}

void Environment::setDayLength(float sec) 
{ 

	m_dayLength = sec; 

}

Vec3f Environment::getSunColor()
{

	float r, g, b;
	float help;

	r=0;
	g=0;
	b=0;

	if(m_time > 0.75 * m_dayLength) 
	{

		help = m_time - 0.75 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1 - help;

	} 
	else if(m_time > 0.5 * m_dayLength) 
	{

		help = m_time - 0.5 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = 1 - help;
		b = 0.5 * (1 - help);

	} 
	else if(m_time > 0.25 * m_dayLength) 
	{

		help = m_time - 0.25 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = help;
		b= 0.5 * help;

	} 
	else 
	{

		help = m_time / (m_dayLength/4);
		r = help;

	}

	return Vec3f(r,g,b);

}

float Environment::getSunIntensity(){

	float help;

	help = m_time / m_dayLength;

	if(help>0.25 && help<0.75){

		help -= 0.25;
		if(help > 0.25){
			help = 0.5 - help;
		}

		return help*3;

	}
	return 0.0;
}

Vec3f Environment::getSunPos(){

	return Vec3f(sin(m_time*2*3.1415/m_dayLength),-cos(m_time*2*3.1415/m_dayLength),0);
}






Vec3f Environment::getcUp()
{

	float help = m_time;

	if(m_time > 0.5 * m_dayLength)
	{

		help = m_dayLength - m_time; 

	}

	return Vec3f(0, 0, 0.7 * (help/5));

}

Vec3f Environment::getcWest()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 + (0.25 * help), 0.75 * help, help);

  } 
  else 
  {
    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 * help, 0, 0);
  }

}

Vec3f Environment::getcNorth()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.5)
    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);

  }

}

Vec3f Environment::getcEast()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.5 * (1 - help), 0, 0);

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.25 * help), 0.75 - (0.75 * help), 1 - help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);

  }
}

Vec3f Environment::getcSouth()
{

	return getcNorth();

}

Vec3f Environment::getcDown()
{

	return Vec3f(0,0,0);

}