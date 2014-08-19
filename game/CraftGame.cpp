#include "CraftGame.hpp"

#include <functional>
#include <chrono>
#include <cmath>

using namespace lumina;
using namespace std;

CraftGame::CraftGame()
    :m_player(NULL), m_mapView(m_map, m_camera)
{
  m_running = true;
}

CraftGame::~CraftGame()
{
  stop();

}

void CraftGame::stop()
{
    // C++0x allow to double delete null pointer but some old compiler might not allow.
    // Therefore, we have to protect double deletion.
    if ( m_player !=NULL ) {
        delete m_player;
        m_player = NULL;
    }
}


void CraftGame::init() {

  // configure window
  m_window.setTitle("CraftGame ComputerGrafikPraktikum 2014");
  m_window.setVersionHint(3, 3);
  m_cheatmode = false;

  if (m_player==NULL) {
      m_player = new Player( m_map );
  }

  // add event callback (capture by reference
  m_window.addEventCallback(
    [&](InputEvent e) { return m_player->processEvent(e, m_window, m_cheatmode); });
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
        m_camera.updateFromPlayer(m_player->getPosition(), m_player->getDirection());
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

  auto now = chrono::system_clock::now();

  uint counter = 0;

  // the chunk where the player/the camera is in
  Vec2i activeChunk = m_map.getChunkPos(m_camera.get_position());
  Vec2i oldChunk = activeChunk;

  m_gBufferNormal.create(m_window.getSize(), TexFormat::RGB8);
  m_gBufferDepth.create(m_window.getSize(), TexFormat::R32F);
  m_gBuffer.create();
  m_gBuffer.attachColor(0, m_gBufferNormal);
  m_gBuffer.attachColor(1, m_gBufferDepth);

  m_lBufferTex.create(m_window.getSize(), TexFormat::RGBA8);
  m_lBuffer.create();
  m_lBuffer.attachColor(0, m_lBufferTex);

  m_fBufferTex.create(m_window.getSize(), TexFormat::RGB8);
  m_fBuffer.create();
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

  Tex2D zBuffer;
  zBuffer.create(m_window.getSize(), TexFormat::D32);
  m_gBuffer.attachDepth(zBuffer);
  m_fBuffer.attachDepth(zBuffer);

  // generate the first chunks
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position());

  float sum = 0;

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    auto diff = chrono::system_clock::now() - now;
    float s = chrono::duration_cast<chrono::milliseconds>(diff).count()
              / 1000.f;

    sum += s;

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
      m_player->update();
      m_camera.updateFromPlayer(m_player->getPosition(), m_player->getDirection());
    }

    auto viewMatrix = m_camera.get_matrix();
    auto projectionMatrix = m_camera.get_ProjectionMatrix(m_window);

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
    });

    // third pass (final)
    m_fBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);
      m_mapView.drawFinalPass(viewMatrix, projectionMatrix);
    });

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      m_fBufferTex.prime(0, [&](HotTex2D& hotT) {
        tempP.prime([&](HotProgram& hotP) {
          hotP.draw(hotT, m_fullScreenQuad, PrimitiveType::TriangleStrip);
        });
      });

      // m_envir.draw(viewMatrix, projectionMatrix);
    });

    // swap buffer
    hotContext.swapBuffer();

    counter++;
  }
}
