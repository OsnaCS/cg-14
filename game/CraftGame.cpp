#include "CraftGame.hpp"

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

  // add event callback (capture by reference)
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
  m_window.resize(Vec2i(800, 600));
}

void CraftGame::start() {
  // open the window (we need to call init before!)
  m_window.open();

  // obtain and create render context
  auto& renderContext = m_window.getRenderContext();
  renderContext.create();

  // we just need one context, so we can prime it here just once
  renderContext.prime([this](HotRenderContext& hotContext) {
    this->run(hotContext);
  });
}

void CraftGame::run(lumina::HotRenderContext& hotContext) {
  // create VertexSeq that represents a triangle
  VertexSeq triangle;
  triangle.create(5, 3);  // 2+3 floats, 3 vertices
  triangle.prime<Vec2f, Color32f>([](HotVertexSeq<Vec2f, Color32f>& hot){
    hot.vertex[0].set(Vec2f(-1.f, -1.f), Color32f(1, 0, 0));
    hot.vertex[1].set(Vec2f( 0.f,  1.f),  Color32f(0, 1, 0));
    hot.vertex[2].set(Vec2f( 1.f, -1.f),  Color32f(0, 0, 1));
  });


  // load and compile vertex and fragment shader
  VShader vs;
  vs.compile(loadShaderFromFile("shader/CraftGame.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/CraftGame.fsh"));

  // create program and link the two shaders
  Program p;
  p.create(vs, fs);


  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    // poll events
    m_window.update();
    

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));

      // prime program to use it
      p.prime([&](HotProgram& hot) {
        // draw the triangle
        hot.draw(triangle, PrimitiveType::Triangle);
      });
    });
    

    // swap buffer
    hotContext.swapBuffer();
  }
}
