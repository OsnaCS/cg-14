#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Environment.hpp"
#include "MapView.hpp"
#include "ChunkGenerator.hpp"
#include "PlayerView.hpp"

class CraftGame {
public:
  CraftGame();

  void init();
  void start();

private:
  lumina::Window m_window;
  ChunkGenerator m_chunkGenerator;
  bool m_running;
  Camera m_camera;
  Player m_player;
  bool m_cheatmode;
  Map m_map;
  Environment m_envir;
  MapView m_mapView;
  PlayerView m_playerView;

  Tex2D m_gBufferNormal;
  Tex2D m_gBufferDepth;
  FrameBuffer m_gBuffer;
  VertexSeq m_fullScreenQuad;
  Tex2D m_lBufferTex;
  FrameBuffer m_lBuffer;
  Tex2D m_fBufferTex;
  FrameBuffer m_fBuffer;

  void run(lumina::HotRenderContext& hotContext);
  lumina::EventResult onEvent(lumina::InputEvent e);
  void updateComponents(float delta);
};
