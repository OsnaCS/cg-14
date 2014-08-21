#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"

using namespace lum;

class Environment 
{

public:

	Environment(Camera& camera);

  void draw(Mat4f viewMat, Mat4f projMat);
  void drawLightingPass(Mat4f viewMat, Mat4f projMat, TexCont& gBuffer);
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
  Program m_programSphere;
  Program m_programSun;
  Program m_programMoon;
  Program m_lightingPass;
  Camera& m_camera;

  float m_dayLength;
  float m_time;
  int m_day;

  Vec3f getcUp();
  Vec3f getcWest();
  Vec3f getcNorth();
  Vec3f getcEast();
  Vec3f getcSouth();
  Vec3f getcDown();

};