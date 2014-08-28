#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include <vector>

using namespace lum;

class Environment 
{

public:

	Environment(Camera& camera);

  void draw(Mat4f viewMat, Mat4f projMat);
  void drawLightingPass(Mat4f viewMat, Mat4f projMat, TexCont& gBuffer);
  void drawCloudNormalPass(Mat4f viewMat, Mat4f projMat);
  void drawCloudFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBufferTex, Tex2D& gBufferDepth);
  void init();

  void update(float delta);

  float getAmbientIntensity();

  void setDayLength(float sec);

  float getTime()
  {return m_time;}

  float getDayLength()
  {return m_dayLength;}
  
  Vec3f getSkyLightColor();
  float getSkyLightIntensity();
  Vec3f getSkyLightDir();

private:

  VertexSeq<Vec3f> m_sphere;
  VertexSeq<Vec3f, Vec2f> m_sun;
  VertexSeq<Vec3f, Vec2f> m_moon;
  VertexSeq<Vec3f,Vec3f,Vec2f> m_cloudBig;
  VertexSeq<Vec3f,Vec3f,Vec2f> m_cloudNormal;
  VertexSeq<Vec3f,Vec3f,Vec2f> m_cloudSmall;
  Program m_programSphere;
  Program m_programSun;
  Program m_programCloud;
  Program m_programMoon;
  Program m_lightingPass;
  Program m_programFinalCloud;
  Camera& m_camera;

  float m_dayLength;
  float m_time;
  int m_day;
  float m_orbitAngle;
  float m_phase;
  float m_sunAxis;
  float m_moonAxis;
  float m_pulse;
  std::vector<std::pair<Vec3f, int>> m_cloudPosition;

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