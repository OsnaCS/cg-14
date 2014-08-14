#pragma once

#include "lumina/lumina.hpp"

using namespace lum;

class Environment 
{

public:
	
  void draw(Mat4f viewMat, Mat4f projMat);

  void init();

private:

  VertexSeq m_sphere;
  Program m_program;

};