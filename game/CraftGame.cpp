#include "CraftGame.hpp"
#include "Pyramid.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

CraftGame::CraftGame() {
  m_running = true;
  m_rotationAngleStep = 0.03f;
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
    } else if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Up) {
      auto pos = m_camera.getPosition();
      pos.y += 0.5f;
      m_camera.setPosition(pos);
    } else if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Down) {
      auto pos = m_camera.getPosition();
      pos.y -= 0.5f;
      m_camera.setPosition(pos);
    } else if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::P) {
      if (m_rotationAngleStep > 0.000001f) {
        m_rotationAngleStep = 0.0f;
      } else {
        m_rotationAngleStep = 0.03f;
      }
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
  VertexSeq triangle = createPyramid(.5f, .5f);
  /*triangle.create(5, 3);  // 2+3 floats, 3 vertices
  triangle.prime<Vec2f, Color32f>([](HotVertexSeq<Vec2f, Color32f>& hot){
    hot.vertex[0].set(Vec2f(-1.f, -1.f), Color32f(1, 0, 0));
    hot.vertex[1].set(Vec2f( 0.f,  1.f),  Color32f(0, 1, 0));
    hot.vertex[2].set(Vec2f( 1.f, -1.f),  Color32f(0, 0, 1));
  });*/


  // load and compile vertex and fragment shader
  VShader vs;
  vs.compile(loadShaderFromFile("shader/CraftGame.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/CraftGame.fsh"));

  // create program and link the two shaders
  Program p;
  p.create(vs, fs);

  float angle = 0.f;
  p.perFragProc.enableDepthTest();
  p.perFragProc.enableDepthWrite();
  p.perFragProc.setDepthFunction(DepthFunction::Greater);

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {

    // poll events
    m_window.update();
    
    angle += m_rotationAngleStep;

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(0.f);

      // prime program to use it
      p.prime([&](HotProgram& hot) {

        hot.uniform["u_world"] = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.f, 1.0f, 0.f), angle));
        hot.uniform["u_view"] = this->m_camera.get_matrix();
        hot.uniform["u_projection"] = this->m_camera.get_ProjectionMatrix();

        // draw the triangle
        hot.draw(triangle, PrimitiveType::Triangle);
      });
    });
    

    // swap buffer
    hotContext.swapBuffer();
  }
}
