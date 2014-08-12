#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"

class CraftGame {
public:
  CraftGame();

  void init();
  void start();

private:
  lumina::Window m_window;
  float m_rotationAngleStep;
  bool m_running;
  Camera m_camera;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
