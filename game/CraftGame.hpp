#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Map.hpp"

class CraftGame {
public:
  CraftGame();

  void init();
  void start();

private:
  lumina::Window m_window;
  bool m_running;
  Camera m_camera;
  Player m_player;
  bool m_cheatmode;
  Map m_map;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
