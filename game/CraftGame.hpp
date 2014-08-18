#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Environment.hpp"
#include "MapView.hpp"
#include "ChunkGenerator1.hpp"

class CraftGame {
public:
  CraftGame();
  ~CraftGame();

  void init();
  void start();
  void stop();

private:
  lumina::Window m_window;
  ChunkGenerator1 m_chunkGenerator;
  bool m_running;
  Camera m_camera;
  Player* m_player;
  bool m_cheatmode;
  Map m_map;
  Environment m_envir;
  MapView m_mapView;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
