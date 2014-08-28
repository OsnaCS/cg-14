#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Environment.hpp"
#include "MapView.hpp"
#include "ChunkGenerator.hpp"
#include "PlayerView.hpp"
#include "FPSCounter.hpp"

class CraftGame {
public:
  CraftGame();

  void init(Vec2i size, bool fullscreen);
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

  bool m_pause;
  Vec2f m_pos;
  int m_button;
  Vec2i m_size;
  bool m_optionen;

  Tex2D m_gBufferNormal;
  Tex2D m_gBufferDepth;
  FrameBuffer m_gBuffer;
  VertexSeq<Vec2f> m_fullScreenQuad;
  VertexSeq<Vec2f, Vec2f> m_fullScreenQuad2;
  Tex2D m_lBufferTex;
  FrameBuffer m_lBuffer;
  Tex2D m_fBufferTex;
  FrameBuffer m_fBuffer;
  Tex2D m_fxaaTex;
  FrameBuffer m_fxaaBuffer;
  FPSCounter m_fps;

  Tex2D m_resTex;
  Tex2D m_saveTex;
  Tex2D m_loadTex;
  Tex2D m_exitTex;
  Tex2D m_optionsTex;
  Tex2D m_resSh;
  
  void run(lumina::HotRenderContext& hotContext);
  lumina::EventResult onEvent(lumina::InputEvent e);
  void updateComponents(float delta);
};
