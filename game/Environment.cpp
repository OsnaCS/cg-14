#include "Environment.hpp"

#include <math.h>

Environment::Environment() : m_dayLength(10), m_time(0) {}

void Environment::draw(Mat4f viewMat, Mat4f projMat)
{

  float a = 0;

	m_program.prime([&](HotProgram& hotprog)
	{

		viewMat.setColumn(3, Vec4f(0,0,0,1));
		// Mat4f mat;
		// mat.setToIdentity();
		// mat.setDiagonal(Vec4f(2,2,2,1));

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

    a = m_time * 3.1415 * 2 / (m_dayLength);


    float cWinkel = cos(a);
    float sWinkel = sin(a);


    hotprog.uniform["u_sinus"] = sWinkel;
    hotprog.uniform["u_cosinus"] = cWinkel;
    hotprog.uniform["u_color"] = getSunColor();

    viewMat.setColumn(3, Vec4f(0,0,0,1));
    // Mat4f mat;
    // mat.setToIdentity();
    // mat.setDiagonal(Vec4f(2,2,2,1));

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

  m_sun.create(3 + 3, 8, 9);
  m_sun.prime<Vec3f, Vec3f>([](HotVertexSeq<Vec3f, Vec3f>& hot)
  {

    hot.vertex[0].set(Vec3f(-0.5, -10, 0.5), Vec3f(1,1,0));
    hot.vertex[1].set(Vec3f(-0.5, -10, -0.5), Vec3f(1,1,0));
    hot.vertex[2].set(Vec3f(0.5, -10, 0.5), Vec3f(1,1,0));
    hot.vertex[3].set(Vec3f(0.5, -10, -0.5), Vec3f(1,1,0));

    hot.vertex[4].set(Vec3f(-0.5, 10, 0.5), Vec3f(0.75,0.75,0.75));
    hot.vertex[5].set(Vec3f(-0.5, 10, -0.5), Vec3f(0.75,0.75,0.75));
    hot.vertex[6].set(Vec3f(0.5, 10, 0.5), Vec3f(0.75,0.75,0.75));
    hot.vertex[7].set(Vec3f(0.5, 10, -0.5), Vec3f(0.75,0.75,0.75));

    hot.index[0] = 0;
    hot.index[1] = 1;
    hot.index[2] = 2;
    hot.index[3] = 3;
    hot.index[4] = GLIndex::PrimitiveRestart;

    hot.index[5] = 4;
    hot.index[6] = 5;
    hot.index[7] = 6;
    hot.index[8] = 7;

  });

}

void Environment::update(float delta){

	m_time += delta;

	if(m_time > m_dayLength){
		m_time -= m_dayLength;
	}

}

void Environment::setDayLength(float sec) { m_dayLength = sec; }

Vec3f Environment::getSunColor(){
	float r, g, b;
	float help;

	r=0;
	g=0;
	b=0;
	if(m_time > 0.75 * m_dayLength){
		help = m_time - 0.75 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1 - help;
	} else if(m_time > 0.5 * m_dayLength){
		help = m_time - 0.5 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = 1 - help;
		b = 0.5 * (1 - help);
	} else if(m_time > 0.25 * m_dayLength){
		help = m_time - 0.25 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1;
		g = help;
		b= 0.5 * help;

	} else{
		help = m_time / (m_dayLength/4);
		r = help;
	}
	return Vec3f(r,g,b);

}


Vec3f Environment::getcUp(){

	float help = m_time;
	if(m_time > 0.5 * m_dayLength){

		help = m_dayLength - m_time; 

	}

	return Vec3f(0, 0, 0.7 * (help/5));

}

Vec3f Environment::getcWest(){
	float r, g, b, help;

  if(m_time > 0.75 * m_dayLength) {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } else if(m_time > 0.5 * m_dayLength) {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } else if(m_time > 0.25 * m_dayLength) {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 + (0.25 * help), 0.75 * help, help);

  } else {
    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.5 * help, 0, 0);
  }

}

Vec3f Environment::getcNorth(){

	float r, g, b, help;

  if(m_time > 0.75 * m_dayLength) {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.)
    return Vec3f(0.25 * (1 - help), 0.25 * (1 - help), 0.5 * (1-help));

  } else if(m_time > 0.5 * m_dayLength) {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.5 * help), 0.75 - (0.5 * help), 1 - 0.5 * help);

  } else if(m_time > 0.25 * m_dayLength) {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(0,0,0.5)
    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } else {
    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);
  }

}

Vec3f Environment::getcEast(){
	float r, g, b, help;

  if(m_time > 0.75 * m_dayLength) {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);

    //(.5,0,0)
    return Vec3f(0.5 * (1 - help), 0, 0);

  } else if(m_time > 0.5 * m_dayLength) {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.75 - (0.25 * help), 0.75 - (0.75 * help), 1 - help);

  } else if(m_time > 0.25 * m_dayLength) {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 + (0.5 * help), 0.25 + (0.5 * help), 0.5 + (0.5 * help));

  } else {
    help = m_time;
    help = help / (m_dayLength / 4);

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);
  }
}

Vec3f Environment::getcSouth(){
	return getcNorth();
}

Vec3f Environment::getcDown(){

	return Vec3f(0,0,0);

}