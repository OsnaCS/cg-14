#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Environment.hpp"
#include "MapView.hpp"
#include "ChunkGenerator.hpp"

class CraftGame {
public:
  CraftGame();
  ~CraftGame();

  void init();
  void start();
  void stop();

private:
  lumina::Window m_window;
  ChunkGenerator m_chunkGenerator;
  bool m_running;
  Camera m_camera;
  Player* m_player;
  bool m_cheatmode;
  Map m_map;
  Environment m_envir;
  MapView m_mapView;

  Tex2D m_gBufferNormal;
  Tex2D m_gBufferPosition;
  FrameBuffer m_gBuffer;
  VertexSeq m_fullScreenQuad;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
