#include "CraftGame.hpp"

#include <functional>
#include <chrono>
#include <cmath>

using namespace lumina;
using namespace std;

CraftGame::CraftGame()
    :m_camera(m_window)
    ,m_player(m_map)
    ,m_envir(m_camera)
    ,m_mapView(m_map, m_camera, m_envir)
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

void CraftGame::updateComponents(float delta) {

  // update game window
  m_window.update();

  // update environment
  m_envir.update(delta);

  // poll events
  if(m_cheatmode){
    m_camera.update();
  }else{
    m_player.update();
    m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
  }

  // generate new chunks if neccessary
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position(), m_mapView);

  if(m_mapView.size() > 100) {

    m_mapView.clearMapView(m_map.getChunkPos(m_camera.get_position()));
  }
}

void CraftGame::run(lumina::HotRenderContext& hotContext) {


  m_envir.init();
  m_mapView.init();
  m_playerView.init();
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position(), m_mapView);

  m_gBufferNormal.create(m_window.getSize(), TexFormat::RGB8);
  m_gBufferDepth.create(m_window.getSize(), TexFormat::R32F);
  m_gBuffer.create(m_window.getSize());
  m_gBuffer.attachColor(0, m_gBufferNormal);
  m_gBuffer.attachColor(1, m_gBufferDepth);
  TexCont gCont;
  gCont.addTexture(0, m_gBufferNormal);
  gCont.addTexture(1, m_gBufferDepth);

  m_lBufferTex.create(m_window.getSize(), TexFormat::RGBA8);
  m_lBufferTex.params.filterMode = TexFilterMode::Nearest;
  m_lBuffer.create(m_window.getSize());
  m_lBuffer.attachColor(0, m_lBufferTex);

  m_fBufferTex.create(m_window.getSize(), TexFormat::RGB8);
  m_fBuffer.create(m_window.getSize());
  m_fBuffer.attachColor(0, m_fBufferTex);

  m_fullScreenQuad.create(2, 4);
  m_fullScreenQuad.prime<Vec2f>([&](HotVertexSeq<Vec2f>& hotSeq) {
    hotSeq.vertex[0] = Vec2f(-1, -1);
    hotSeq.vertex[1] = Vec2f(1, -1);
    hotSeq.vertex[2] = Vec2f(-1, 1);
    hotSeq.vertex[3] = Vec2f(1, 1);
  });

  VShader tempVS;
  tempVS.compile(loadShaderFromFile("shader/TempShader.vsh"));
  FShader tempFS;
  tempFS.compile(loadShaderFromFile("shader/TempShader.fsh"));

  Program tempP;
  tempP.create(tempVS, tempFS);

  RenderBuffer zBuf;
  zBuf.create(m_window.getSize(), RenderBufferType::Depth32);

  m_fBuffer.attachRenderBuffer(zBuf);
  m_gBuffer.attachRenderBuffer(zBuf);
  m_lBuffer.attachRenderBuffer(zBuf);

  // time measurement
  float sum = 0;
  auto now = chrono::system_clock::now();

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {

    // measure time
    auto diff = chrono::system_clock::now() - now;
    float delta = chrono::duration_cast<chrono::milliseconds>(diff).count() / 1000.f;
    now = chrono::system_clock::now();
    sum += delta;

    // print FPS
    if(sum > 2) {
      slog("FPS:", 1 / delta);
      sum -= 2;
    }

    // update game components
    updateComponents(delta);

    auto viewMatrix = m_camera.get_matrix();
    auto projectionMatrix = m_camera.get_ProjectionMatrix();

    // first pass (geometry)
    m_gBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearColor(1, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      m_mapView.drawNormalPass(viewMatrix, projectionMatrix);
    });

    // second pass (lighting)
    m_lBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 0));

      m_envir.drawLightingPass(viewMatrix, projectionMatrix, gCont);
    });

    // third pass (final)
    m_fBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));

      // TODO: remove
      hotFB.clearDepth(1.f);

      m_envir.draw(viewMatrix, projectionMatrix);



      m_mapView.drawFinalPass(viewMatrix, projectionMatrix, m_lBufferTex, m_gBufferDepth);
    });

    hotContext.getDefaultFrameBuffer().enableBlending(0);

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen

      hotFB.clearColor(0, Color32fA(0, 0, 0, 0));
      hotFB.clearDepth(1.f);
      
      m_fBufferTex.prime(0, [&](HotTex2D& hotT) {
        tempP.prime([&](HotProgram& hotP) {
          hotP.draw(hotT, m_fullScreenQuad, PrimitiveType::TriangleStrip);
        });
      });

      m_playerView.draw();
    });

    // swap buffer
    hotContext.swapBuffer();
  }
}
