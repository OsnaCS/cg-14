#pragma once

#include "lumina/lumina.hpp"

using namespace lum;

class Environment 
{

public:

	Environment();

  void draw(Mat4f viewMat, Mat4f projMat);

  void init();

  void update(float delta);

  void setDayLength(float sec);

  Vec3f getSunColor();

private:

  VertexSeq m_sphere;
  VertexSeq m_sun;
  Program m_program;
  Program m_program2;
  float m_dayLength;
  float m_time;

};