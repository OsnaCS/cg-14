#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include "Environment.hpp"

class CraftGame {
public:
  CraftGame();

  void init();
  void start();

private:
  lumina::Window m_window;
  bool m_running;
  Camera m_camera;
  Map m_map;
  Environment m_envir;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
