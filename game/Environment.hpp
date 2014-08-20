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

  void setExactness(int e);

private:

  VertexSeq m_sphere;
  VertexSeq m_sun;
  VertexSeq m_moon;
  VertexSeq m_moon1;
  VertexSeq m_moon2;
  VertexSeq m_moon3;
  VertexSeq m_moon4;
  VertexSeq m_moon5;
  VertexSeq m_moon6;
  VertexSeq m_moon7;
  VertexSeq m_moon8;
  VertexSeq m_moon9;
  VertexSeq m_moon10;
  Program m_program;
  Program m_program2;
  float m_dayLength;
  float m_time;
  int m_day;
  int m_exactness;

  Vec3f getcUp();
  Vec3f getcWest();
  Vec3f getcNorth();
  Vec3f getcEast();
  Vec3f getcSouth();
  Vec3f getcDown();

};