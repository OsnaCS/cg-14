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
  ChunkGenerator cg;
  cg.chunkGeneration(m_map,{0,0,0});
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
        hot.uniform["u_lightsource"] = Vec3f(sin(sum*2),-1,cos(sum*2));

        m_mapView.draw(hot);
    
      });
    });

    // swap buffer
    hotContext.swapBuffer();

    counter++;
  }
}
