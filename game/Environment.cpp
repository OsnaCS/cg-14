#include "Environment.hpp"

#include <math.h> 

void Environment::draw(Mat4f viewMat, Mat4f projMat)
{

	m_program.prime([&](HotProgram& hotprog)
	{

		viewMat.setColumn(3, Vec4f(0,0,0,1));
		Mat4f mat;
		mat.setToIdentity();
		mat.setDiagonal(Vec4f(2,2,2,1));

		hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);

	});

  m_program2.prime([&](HotProgram& hotprog)
  {

    viewMat.setColumn(3, Vec4f(0,0,0,1));
    Mat4f mat;
    mat.setToIdentity();
    mat.setDiagonal(Vec4f(2,2,2,1));

    hotprog.uniform["u_transform"] = projMat * viewMat;
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

  m_sun.create(3 + 3, 4);
  m_sun.prime<Vec3f, Vec3f>([](HotVertexSeq<Vec3f, Vec3f>& hot)
  {

    hot.vertex[0].set(Vec3f(0.5, 4.5, 10), Vec3f(1,1,0));
    hot.vertex[1].set(Vec3f(-0.5, 4.5, 10), Vec3f(1,1,0));
    hot.vertex[2].set(Vec3f(0.5, 5.5, 10), Vec3f(1,1,0));
    hot.vertex[3].set(Vec3f(-0.5, 5.5, 10), Vec3f(1,1,0));

  });

}

void Environment::setDayLength(float sec) { m_dayLength = sec; }
