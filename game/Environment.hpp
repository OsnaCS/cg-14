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

private:

  VertexSeq m_sphere;
  VertexSeq m_sun;
  Program m_program;
  Program m_program2;
  Program m_lightingPass;
  Camera& m_camera;
};