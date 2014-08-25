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

  float getTime()
  {return m_time;}

  float getDayLength()
  {return m_dayLength;}
  
  Vec3f getSkyLightColor();
  float getSkyLightIntensity();
  Vec3f getSkyLightDir();

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
  float m_orbitAngle;
  float m_phase;
  float m_sunAxis;
  float m_moonAxis;

  Vec3f getSunColor();
  float getSunIntensity();
  Vec3f getSunPos();

  Vec3f getMoonColor();
  float getMoonIntensity();
  Vec3f getMoonPos();

  Vec3f getcUp();
  Vec3f getcWest();
  Vec3f getcNorth();
  Vec3f getcEast();
  Vec3f getcSouth();
  Vec3f getcDown();

};