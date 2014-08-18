#pragma once

#include "lumina/lumina.hpp"

using namespace lum;

class Environment 
{

public:

  void draw(Mat4f viewMat, Mat4f projMat);

  void init();

  void setDayLength(float sec);

private:

  VertexSeq m_sphere;
  VertexSeq m_sun;
  Program m_program;
  Program m_program2;
  float m_dayLength;
  float m_time;

};