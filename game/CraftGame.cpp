#include "CraftGame.hpp"
#include "ChunkView.hpp"
#include "ChunkGenerator.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

CraftGame::CraftGame() {
  m_running = true;

  ChunkGenerator cg;
  cg.chunkGeneration(m_map,{0,0,0});

}

void CraftGame::init() {

  // configure window
  m_window.setTitle("CG Praktikum 2014 :)");
  m_window.setVersionHint(3, 3);

  // add event callback (capture by reference
  m_window.addEventCallback(
    [&](InputEvent e) { return m_camera.processEvent(e, m_window); });
  m_window.addEventCallback([&](InputEvent e) {
    // if the inputType was a KeyInput and the key was just pressed and the
    // key was Escape -> set m_running to false to stop program
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Escape) {
      m_running = false;
      return EventResult::Processed;
    }
    return EventResult::Skipped;
  });

  // resize window
  m_window.resize(Vec2i(1280, 1024));
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
  // load and compile vertex and fragment shader
  VShader vs;
  vs.compile(loadShaderFromFile("shader/CraftGame.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/CraftGame.fsh"));

  // create program and link the two shaders
  Program p;
  p.create(vs, fs);

  p.perFragProc.enableDepthTest();
  p.perFragProc.enableDepthWrite();
  p.perFragProc.setDepthFunction(DepthFunction::Less);
  p.primitiveProc.enableCulling();

  auto now = chrono::system_clock::now();

  uint counter = 0;

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    auto diff = chrono::system_clock::now() - now;
    float s = chrono::duration_cast<chrono::milliseconds>(diff).count()
              / 1000.f;

    if(counter % 100 == 0) {
      slog("FPS:", 1 / s);
    }

    now = chrono::system_clock::now();

    // poll events
    m_window.update();
    m_camera.update();

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      m_envir.draw(m_camera.get_matrix(), m_camera.get_ProjectionMatrix(m_window));
      // prime program to use it
      p.prime([&](HotProgram& hot) {

        hot.uniform["u_view"] = this->m_camera.get_matrix();
        hot.uniform["u_projection"] = this->m_camera.get_ProjectionMatrix(m_window);

        Chunk& currentChunk = m_map.getChunk(Vec2i(0, 0));
        ChunkView cV1(currentChunk, Vec2i(0, 0));

        cV1.draw(hot);
        ChunkView cV2(currentChunk, Vec2i(-1, 0));

        cV2.draw(hot);
        ChunkView cV3(currentChunk, Vec2i(1, 0));

        cV3.draw(hot);
        ChunkView cV4(currentChunk, Vec2i(0, -1));

        cV4.draw(hot);
        ChunkView cV5(currentChunk, Vec2i(0, 1));

        cV5.draw(hot);

        /*        ChunkView cV6(currentChunk, Vec2i(1, 1));

        cV6.draw(hot);
                ChunkView cV7(currentChunk, Vec2i(-1, 1));

        cV7.draw(hot);
                ChunkView cV8(currentChunk, Vec2i(1, -1));

        cV8.draw(hot);
                ChunkView cV9(currentChunk, Vec2i(-1, -1));

        cV9.draw(hot);*/

      });
    });

    // swap buffer
    hotContext.swapBuffer();

    counter++;
  }
}
