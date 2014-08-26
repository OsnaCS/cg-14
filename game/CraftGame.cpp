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
    ,m_pause(false)
{
  m_running = true;
}

void CraftGame::init() {

  // configure window
  m_window.setTitle("CraftGame ComputerGrafikPraktikum 2014");
  m_window.setVersionHint(3, 3);
  m_cheatmode = false;

  // add event callback (capture by reference
  m_window.addEventCallback([&](InputEvent e) 
  {
    if(!m_pause) 
    {
      return m_player.processEvent(e, m_window, m_cheatmode);
    }

    return EventResult::Skipped;

  });

  m_window.addEventCallback([&](InputEvent e) 
  { 
    if(!m_pause)
    {
      return m_camera.processEvent(e, m_window); 
    }

    return EventResult::Skipped;

  });

  m_window.addEventCallback([&](InputEvent e) 
  {
    // if the inputType was a KeyInput and the key was just pressed and the
    // key was Escape -> set m_running to false to stop program
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Escape) {
      m_running = false;
      return EventResult::Processed;
    }
    // if the keyInpuit is k
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::K  && !m_pause) {
      if(m_cheatmode)
      {
        m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
        m_cheatmode = false;
      }
      else
      {
        m_cheatmode = true;
      }
      return EventResult::Processed;
    }

    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::P)
    {

      if(m_pause == false)
      {
        m_pause = true;
      }
      else
      {
        m_pause = false;
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

  m_fps.printFPS(delta);

  // update game window
  m_window.update();

  // update environment
  if(!m_pause)
  {
    m_envir.update(delta);
  }

  // poll events
  if(m_cheatmode)
  {
    m_camera.update();
  }
  else
  {
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

  ImageBox image_save = loadJPEGImage("gfx/save.jpg");
  m_colorTexture.create(Vec2i(680,90), TexFormat::RGB8, image_save.data());
  m_colorTexture.params.filterMode = TexFilterMode::Trilinear;
  m_colorTexture.params.useMipMaps = true;

  m_envir.init();
  m_mapView.init();
  m_playerView.init();
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position(), m_mapView);

  //
  m_gBufferNormal.create(m_window.getSize(), TexFormat::RGB8);
  m_gBufferDepth.create(m_window.getSize(), TexFormat::R32F);
  m_gBuffer.create(m_window.getSize());
  m_gBuffer.attachColor(0, m_gBufferNormal);
  m_gBuffer.attachColor(1, m_gBufferDepth);
  TexCont gCont;
  gCont.addTexture(0, m_gBufferNormal);
  gCont.addTexture(1, m_gBufferDepth);

  //
  m_lBufferTex.create(m_window.getSize(), TexFormat::RGBA8);
  m_lBufferTex.params.filterMode = TexFilterMode::Nearest;
  m_lBuffer.create(m_window.getSize());
  m_lBuffer.attachColor(0, m_lBufferTex);

  //Texture for FXAA:
  m_fxaaTex.create(m_window.getSize(), TexFormat::RGB8);
  m_fxaaBuffer.create(m_window.getSize());
  m_fxaaBuffer.attachColor(0,m_fxaaTex); //unterschiedliche attachmentpoints: weil rgb-textur ->"color"
  
  //creating the fxaa-program:
  VShader fxaaVS;
  fxaaVS.compile(loadShaderFromFile("shader/FXAAShader.vsh"));
  FShader fxaaFS;
  fxaaFS.compile(loadShaderFromFile("shader/FXAAShader.fsh"));
  Program fxaaP;
  fxaaP.create(fxaaVS, fxaaFS);

  //
  m_fBufferTex.create(m_window.getSize(), TexFormat::RGB8);
  m_fBuffer.create(m_window.getSize());
  m_fBuffer.attachColor(0, m_fBufferTex);

  m_fullScreenQuad.create(4);
  m_fullScreenQuad.prime([&](HotVertexSeq<Vec2f>& hotSeq) {
    hotSeq.vertex[0] = Vec2f(-1, -1);
    hotSeq.vertex[1] = Vec2f(1, -1);
    hotSeq.vertex[2] = Vec2f(-1, 1);
    hotSeq.vertex[3] = Vec2f(1, 1);
  });

  m_fullScreenQuad2.create(4);
  m_fullScreenQuad2.prime([&](HotVertexSeq<Vec2f, Vec2f>& hotSeq) {
    hotSeq.vertex[0].set(Vec2f(-680.0/1280.0, 650.0/720.0),Vec2f(0, 0));
    hotSeq.vertex[1].set(Vec2f(-680.0/1280.0, 470.0/720.0),Vec2f(0, 1));
    hotSeq.vertex[2].set(Vec2f(680.0/1280.0, 650.0/720.0),Vec2f(1, 0));
    hotSeq.vertex[3].set(Vec2f(680.0/1280.0, 470.0/720.0),Vec2f(1, 1));
  });

  VShader tempVS;
  tempVS.compile(loadShaderFromFile("shader/TempShader.vsh"));
  FShader tempFS;
  tempFS.compile(loadShaderFromFile("shader/TempShader.fsh"));
  Program tempP;
  tempP.create(tempVS, tempFS);

  VShader menuVS;
  menuVS.compile(loadShaderFromFile("shader/Menu.vsh"));
  FShader menuFS;
  menuFS.compile(loadShaderFromFile("shader/Menu.fsh"));

  Program pMenu;
  pMenu.create(menuVS, menuFS);

  RenderBuffer zBuf;
  zBuf.create(m_window.getSize(), RenderBufferType::Depth32);

  m_fBuffer.attachRenderBuffer(zBuf);
  m_gBuffer.attachRenderBuffer(zBuf);
  m_lBuffer.attachRenderBuffer(zBuf);

  // time measurement
  auto now = chrono::system_clock::now();

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {

    // measure time
    auto diff = chrono::system_clock::now() - now;
    float delta = chrono::duration_cast<chrono::milliseconds>(diff).count() / 1000.f;
    now = chrono::system_clock::now();

    // update game components
    updateComponents(delta);

    // poll events
    m_window.update();

    if(!m_pause)
    {
      m_envir.update(delta);
    }

    if(m_cheatmode)
    {
      m_camera.update();
    }
    else
    {
      m_player.update();
      
      m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
    }

    auto viewMatrix = m_camera.get_matrix();
    auto projectionMatrix = m_camera.get_ProjectionMatrix();

    // first pass (geometry)
    m_gBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearColor(1, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      m_mapView.drawNormalPass(viewMatrix, projectionMatrix);
      m_playerView.drawNormalPass(viewMatrix, projectionMatrix);
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

    // fourth pass (FXAA)
    m_fxaaBuffer.prime([&](HotFrameBuffer& hotFB) {
      fxaaP.prime([&](HotProgram& hotP) {
        // m_window.getSize() gives Vec2i back, cast to Vec2f and inverse
        hotP.uniform["R_inverseFilterTextureSize"] = Vec2f(1/(m_window.getSize().x + 0.0f), 1/(m_window.getSize().y + 0.0f));
        m_fBufferTex.prime(0, [&](HotTex2D& hotT) {
          hotP.draw(hotT, m_fullScreenQuad, PrimitiveType::TriangleStrip);
        });      
      });

      m_playerView.drawFinalPass(viewMatrix, projectionMatrix, m_camera, m_lBufferTex);
    });

    hotContext.getDefaultFrameBuffer().enableBlending(0);

    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) 
    {
      // clear the background color of the screen

      hotFB.clearColor(0, Color32fA(0, 0, 0, 0));
      hotFB.clearDepth(1.f);
      
      m_fxaaTex.prime(0, [&](HotTex2D& hotT) {
        tempP.prime([&](HotProgram& hotP) {

          hotP.uniform["u_pause"] = m_pause;
          hotP.draw(hotT, m_fullScreenQuad, PrimitiveType::TriangleStrip);
       
        });

      });

      // Für Blur
      m_fBufferTex.prime(0, [&](HotTex2D& hotT)
      // Für Textur
      //m_colorTexture.prime(0, [&](HotTex2D& hotT)
      {

        pMenu.prime([&](HotProgram& hotP)
        {

          if(m_pause)
          {
            hotP.uniform["u_offset"] = (float)0.0;

            hotP.draw(hotT, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-0.4;
            hotP.draw(hotT, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-0.8;
            hotP.draw(hotT, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-1.2;
            hotP.draw(hotT, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

          }
       
        });

      });

      m_playerView.draw(viewMatrix, projectionMatrix);
    });

    // swap buffer
    hotContext.swapBuffer();

  }
}
