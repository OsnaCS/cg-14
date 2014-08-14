#include "Environment.hpp"

void Environment::draw(Mat4f viewMat, Mat4f projMat)
{

	m_program.prime([&](HotProgram& hotprog){
		viewMat.setColumn(3, Vec4f(0,0,-10,1));
		Mat4f mat;
		mat.setToIdentity();
		mat.setDiagonal(Vec4f(2,2,2,1));
		hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.uniform["u_light"] = 0.5f;
		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);

	});

}

void Environment::init()
{

	VShader vs;
	vs.compile(loadShaderFromFile("shader/Environment.vsh"));
	FShader fs;
	fs.compile(loadShaderFromFile("shader/Environment.fsh"));

  // create program and link the two shaders
	m_program.create(vs, fs);


  m_sphere.create(3 + 3, 4);
  m_sphere.prime<Vec3f, Vec3f>([](HotVertexSeq<Vec3f, Vec3f>& hot){

  	hot.vertex[0].set(Vec3f(500, 500, 500), Vec3f(1,0,0));
  	hot.vertex[1].set(Vec3f(500, -500, 500), Vec3f(0,1,0));
  	hot.vertex[2].set(Vec3f(-500, 500, 500) , Vec3f(0,0,1));
  	hot.vertex[3].set(Vec3f(-500, -500, 500), Vec3f(0,0,1));
  });
}

