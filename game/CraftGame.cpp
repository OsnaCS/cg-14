#include "CraftGame.hpp"
#include "ChunkView.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

CraftGame::CraftGame() {
  m_running = true;
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
    m_camera.update();

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      // prime program to use it
      p.prime([&](HotProgram& hot) {

        hot.uniform["u_view"] = this->m_camera.get_matrix();
        hot.uniform["u_projection"] = this->m_camera.get_ProjectionMatrix(m_window);

        for(int x = activeChunk.x - 2; x <= activeChunk.x + 2; x++) {
          for(int z = activeChunk.y - 2; z <= activeChunk.y + 2; z++) {

            Chunk& currentChunk = m_map.getChunk(Vec2i(x, z));
            ChunkView cV1(currentChunk, Vec2i(x, z));
            cV1.draw(hot);
          }
        }

      });
    });

    // swap buffer
    hotContext.swapBuffer();

    counter++;
  }
}
