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
		hotprog.uniform["u_light"] = 1.0f;
		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);
	//	hotprog.draw(m_sun, PrimitiveType::TriangleStrip);

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
  	// hot.vertex[6].set(Vec3f(0, 1, -1), Vec3f(0,0,1));
  	// hot.vertex[7].set(Vec3f(1, 0, -1), Vec3f(0,0,1));

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

 //  int n = 1;

 //  #define PI 3.14159265

	// double alpha = 2 * PI / n;

  // m_sun.create(3 + 3, 4, 5);//n*(n/2-1)
  // m_sun.prime<Vec3f, Vec2f>([&](HotVertexSeq<Vec3f, Vec2f>& hot)
  // {

  // 	hot.vertex[0].set(Vec3f(1, 1, 20), Vec2f(0,0));
  // 	hot.vertex[1].set(Vec3f(2, 1, 20), Vec2f(0,0));
  // 	hot.vertex[2].set(Vec3f(1, 2, 20) , Vec2f(0,0));
  // 	hot.vertex[3].set(Vec3f(2, 2, 20), Vec2f(0,0));

		// // for(int k = 0; k < n; k++)
		// // {

		// // 	double phi = k * alpha;
		// // 	for(int l = 1; l < 2; l++)
		// // 	{		

		// // 		double theta = l * alpha;

		// // 		hot.vertex[k+(l/2-1)].set(Vec3f(1+sin(theta) * cos(phi), 1+sin(theta) * sin(phi), 1+cos(theta)), Vec2f(1,1));
  // //   		hot.vertex[k+(l/2-1)+1].set(Vec3f(1+sin(theta) * cos(phi + alpha), 1+sin(theta) * sin(phi + alpha), 1+cos(theta)), Vec2f(1,1));
  // //   		hot.vertex[k+(l/2-1)+2].set(Vec3f(1+sin(theta + alpha) * cos(phi + alpha), 1+sin(theta + alpha) * sin(phi + alpha), 1+cos(theta + alpha)) , Vec2f(1,1));
  // //   		hot.vertex[k+(l/2-1)+3].set(Vec3f(1+sin(theta + alpha) * cos(phi), 1+sin(theta + alpha) * sin(phi), 1+cos(theta + alpha)), Vec2f(1,1));
				
  // // 			hot.vertex[0].set(Vec3f(1, 1, 20), Vec2f(0,0));
  // // 			hot.vertex[1].set(Vec3f(2, 1, 20), Vec2f(0,0));
  // // 			hot.vertex[2].set(Vec3f(1, 2, 20) , Vec2f(0.5,1));
  // // 			hot.vertex[3].set(Vec3f(2, 2, 20), Vec2f(1,1));

  // // 			hot.index[k+(l/2-1)] = 0;//k+(l/2-1);
  // // 			hot.index[k+(l/2-1)+1] = 1;//k+(l/2-1)+1;
  // // 			hot.index[k+(l/2-1)+2] = 3;//k+(l/2-1)+3;
  // // 			hot.index[k+(l/2-1)+3] = 2;//k+(l/2-1)+2;
  // // 			hot.index[k+(l/2-1)+4] = 0;//k+(l/2-1);
  // // 			hot.index[k+(l/2-1)+5] = k+(l/2-1)+2;
  // // 			hot.index[k+(l/2-1)+6] = k+(l/2-1)+1;
  // // 			hot.index[k+(k/2-1)+4] = GLIndex::PrimitiveRestart;

		// // 	}

		// // }

  // });

}

