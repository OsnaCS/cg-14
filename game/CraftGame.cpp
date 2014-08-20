#include "CraftGame.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

CraftGame::CraftGame() : 
    m_player(m_map)
    ,m_mapView(m_map, m_camera)
    ,m_playerView(m_player)


{
  m_running = true;
}


void CraftGame::init() {

  // configure window
  m_window.setTitle("CraftGame ComputerGrafikPraktikum 2014");
  m_window.setVersionHint(3, 3);
  m_cheatmode = false;

  // add event callback (capture by reference
  m_window.addEventCallback(
    [&](InputEvent e) { return m_player.processEvent(e, m_window, m_cheatmode); });
  m_window.addEventCallback(
    [&](InputEvent e) { return m_camera.processEvent(e, m_window); });
  m_window.addEventCallback([&](InputEvent e) {
    // if the inputType was a KeyInput and the key was just pressed and the
    // key was Escape -> set m_running to false to stop program
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Escape) {
      m_running = false;
      return EventResult::Processed;
    }
    // if the keyInpuit is k
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::K) {
      if(m_cheatmode){
        m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
        m_cheatmode = false;
      }else{
        m_cheatmode = true;
      }
      return EventResult::Processed;
    }
    return EventResult::Skipped;
  });

  // resize window
  m_window.resize(Vec2i(1280, 720));
}

void CraftGame::start() {
  // open the window (we need to call init before!)
  m_window.open();
  // obtain and create render context
  auto& renderContext = m_window.getRenderContext();
  renderContext.create();
  // we just need one context, so we can prime it here just once
  renderContext.prime(
    [this](HotRenderContext& hotContext) { this->run(hotContext); });
}

void CraftGame::run(lumina::HotRenderContext& hotContext) {

  m_envir.init();
  m_mapView.init();
  m_playerView.init();

  auto now = chrono::system_clock::now();

  uint counter = 0;

  // the chunk where the player/the camera is in
  Vec2i activeChunk = m_map.getChunkPos(m_camera.get_position());
  Vec2i oldChunk = activeChunk;

  // generate the first chunks
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position());

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    auto diff = chrono::system_clock::now() - now;
    float s = chrono::duration_cast<chrono::milliseconds>(diff).count()
              / 1000.f;

    if(counter % 100 == 0) {
      slog("FPS:", 1 / s);
    }

    now = chrono::system_clock::now();

    // update activeChunk and generate new chunks
    activeChunk = m_map.getChunkPos(m_camera.get_position());
    if (oldChunk != activeChunk) {
      oldChunk = activeChunk;
      m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position());
    }

    // poll events
    m_window.update();
    if(m_cheatmode){
      m_camera.update();
    }else{
      m_player.update();
      m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
    }

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      m_envir.draw(m_camera.get_matrix(), m_camera.get_ProjectionMatrix(m_window));
      m_mapView.draw(m_camera.get_matrix(), m_camera.get_ProjectionMatrix(m_window));
      m_playerView.draw();
    });

    // swap buffer
    hotContext.swapBuffer();

    counter++;
  }
}
