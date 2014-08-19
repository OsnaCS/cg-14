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
  float getSunIntensity();
  Vec3f getSunPos();

private:

  VertexSeq m_sphere;
  VertexSeq m_sun;
  VertexSeq m_moon;
  Program m_program;
  Program m_program2;
  float m_dayLength;
  float m_time;
  int m_day;
  bool visible;

  Vec3f getcUp();
  Vec3f getcWest();
  Vec3f getcNorth();
  Vec3f getcEast();
  Vec3f getcSouth();
  Vec3f getcDown();

};