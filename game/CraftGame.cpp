#include "CraftGame.hpp"

#include <functional>
#include <chrono>
#include <cmath>

using namespace lumina;
using namespace std;

CraftGame::CraftGame()
    :m_camera(m_window)
    ,m_player(m_map, m_mapView)
    ,m_envir(m_camera)
    ,m_mapView(m_map, m_camera, m_envir)
    ,m_playerView(m_player)
    ,m_pause(true)
    ,m_pos(0.0,0.0)
    ,m_button(0)
{
  m_running = true;
}

void CraftGame::init(Vec2i size, bool fullscreen) {

  // configure window
  m_window.setTitle("CraftGame ComputerGrafikPraktikum 2014");
  m_window.setVersionHint(3, 3);
  m_window.resize(size);
  m_window.setFullscreen(fullscreen);
  m_cheatmode = false;
  m_size = size;
  m_camera.setScreenRatio(m_size);

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
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Escape) 
    {
      //m_running = false;
      if(m_pause)
      {
        m_pause = false;
        m_window.setCursorMode(CursorMode::Free);
        m_player.resetkeys();
      }
      else
      {
        m_pause = true;
        m_window.setCursorMode(CursorMode::Normal);
        m_player.resetkeys();
      }

      return EventResult::Processed;
    }
    // if the keyInpuit is k
    if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::K  && !m_pause) 
    {

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

      if(m_pause)
      {
        m_pause = false;
        m_window.setCursorMode(CursorMode::Free);
        m_player.resetkeys();
      }
      else
      {
        m_pause = true;
        m_window.setCursorMode(CursorMode::Normal);
        m_player.resetkeys();
      }
      return EventResult::Processed;

    }

    return EventResult::Skipped;

  });

  m_window.addEventCallback([&](InputEvent e) 
  {

    if(e.type == InputType::MouseMovePos && m_pause) 
    {

      m_pos = Vec2f(e.mouseInput.x, e.mouseInput.y);

      // Startposition des ersten Buttons
      float starty = m_size.y/20.0+20;
      // Größe um die die Buttons versetzt nach unten liegen
      float offset = (m_size.y/6.0);
      
      if(m_pos.x >= m_size.x/2-135 && m_pos.x <= m_size.x/2+135 && m_pos.y >= starty && m_pos.y <= starty + 50)
      {
        m_button = 1;
      }
      else if(m_pos.x >= m_size.x/2-135 && m_pos.x <= m_size.x/2+135 && m_pos.y >= starty + offset && m_pos.y <= starty + offset + 50)
      {
        m_button = 2;
      }
      else if(m_pos.x >= m_size.x/2-135 && m_pos.x <= m_size.x/2+135 && m_pos.y >= starty + (offset * 2) && m_pos.y <= starty + (offset * 2) + 50)
      {
        m_button = 3;
      }
      else if(m_pos.x >= m_size.x/2-135 && m_pos.x <= m_size.x/2+135 && m_pos.y >= starty + (offset * 3) && m_pos.y <= starty + (offset * 3) + 50)
      {
        m_button = 4;
      }
      else if(m_pos.x >= m_size.x/2-135 && m_pos.x <= m_size.x/2+135 && m_pos.y >= starty + (offset * 4) && m_pos.y <= starty + (offset * 4) + 50)
      {
        m_button = 5;
      }
      else
      {
        m_button = 0;
      }
      
      return EventResult::Processed;

    }

    return EventResult::Skipped;

  });
  
  m_window.addEventCallback([&](InputEvent e) 
  {

    if(e.type == InputType::LMouseReleased && m_pause)
    {

      switch(m_button)
      {

        case 1:
        {
          m_pause = false;
          m_window.setCursorMode(CursorMode::Free);
          m_player.resetkeys();
          return EventResult::Processed; 
          break;
        }
        case 2: 
        {
          Vec4f posut = m_map.loadWorld("untitled2"); 
          m_player.reset(Vec3f(posut.x,posut.y,posut.z)); 
          m_mapView.resetMapView();
          return EventResult::Processed; 
          break;
        }
        case 3: 
        {
          m_map.saveWorld(); 
          return EventResult::Processed; 
          break;
        }
        case 4: 
        {
          slog("Keine Optionen!");
          return EventResult::Processed; 
          break;
        }
        case 5: 
        {
          m_running = false; 
          return EventResult::Processed; 
          break;
        }
        default: 
        {
          return EventResult::Skipped; 
          break;
        }

      }

    }

    return EventResult::Skipped;

  });

  // resize window
  // m_window.resize(Vec2i(1280, 720));
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
    m_player.update(delta);
    
    m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
  }

  // generate new chunks if neccessary
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position(), m_mapView);

  if(m_mapView.size() > 100) 
  {

    m_mapView.clearMapView(m_map.getChunkPos(m_camera.get_position()));

  }
}

void CraftGame::run(lumina::HotRenderContext& hotContext) {

  ImageBox image_resume = loadPNGImage("gfx/resume.png");
  m_resTex.create(Vec2i(680,90), TexFormat::RGBA8, image_resume.data());
  m_resTex.params.filterMode = TexFilterMode::Trilinear;
  m_resTex.params.useMipMaps = true;

  ImageBox image_ressh = loadPNGImage("gfx/resSh.png");
  m_resSh.create(Vec2i(680,90), TexFormat::RGBA8, image_ressh.data());
  m_resSh.params.filterMode = TexFilterMode::Trilinear;
  m_resSh.params.useMipMaps = true;

  ImageBox image_save = loadPNGImage("gfx/save.png");
  m_saveTex.create(Vec2i(680,90), TexFormat::RGBA8, image_save.data());
  m_saveTex.params.filterMode = TexFilterMode::Trilinear;
  m_saveTex.params.useMipMaps = true;

  ImageBox image_load = loadPNGImage("gfx/load.png");
  m_loadTex.create(Vec2i(680,90), TexFormat::RGBA8, image_load.data());
  m_loadTex.params.filterMode = TexFilterMode::Trilinear;
  m_loadTex.params.useMipMaps = true;

  ImageBox image_exit = loadPNGImage("gfx/exit.png");
  m_exitTex.create(Vec2i(680,90), TexFormat::RGBA8, image_exit.data());
  m_exitTex.params.filterMode = TexFilterMode::Trilinear;
  m_exitTex.params.useMipMaps = true;

  ImageBox image_options = loadPNGImage("gfx/options.png");
  m_optionsTex.create(Vec2i(680,90), TexFormat::RGBA8, image_options.data());
  m_optionsTex.params.filterMode = TexFilterMode::Trilinear;
  m_optionsTex.params.useMipMaps = true;

  m_envir.init();
  m_mapView.init();
  m_playerView.init();
  m_chunkGenerator.chunkGeneration(m_map, m_camera.get_position(), m_mapView);

  //
  m_gBufferNormal.create(m_window.getSize(), TexFormat::RGB32F);
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
  m_lBuffer.enableBlending(0);

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
  m_fullScreenQuad2.prime([&](HotVertexSeq<Vec2f, Vec2f>& hotSeq) 
  {
    hotSeq.vertex[0].set(Vec2f(-680.0/m_size.x, (m_size.y-(m_size.y/10.0))/m_size.y),Vec2f(0, 0));
    hotSeq.vertex[1].set(Vec2f(-680.0/m_size.x, (m_size.y-(m_size.y/10.0)-180.0)/m_size.y),Vec2f(0, 1));
    hotSeq.vertex[2].set(Vec2f(680.0/m_size.x, (m_size.y-(m_size.y/10.0))/m_size.y),Vec2f(1, 0));
    hotSeq.vertex[3].set(Vec2f(680.0/m_size.x, (m_size.y-(m_size.y/10.0)-180.0)/m_size.y),Vec2f(1, 1));
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

    /*
    if(m_cheatmode)
    {
      m_camera.update();
    }
    else
    {
      m_player.update(delta);
      
      m_camera.updateFromPlayer(m_player.getPosition(), m_player.getDirection());
    }
*/
    auto viewMatrix = m_camera.get_matrix();
    auto projectionMatrix = m_camera.get_ProjectionMatrix();

    // first pass (geometry)
    m_gBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearColor(1, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);


      m_mapView.drawNormalPass(viewMatrix, projectionMatrix);

      m_playerView.drawNormalPass(viewMatrix, projectionMatrix, m_camera, m_cheatmode);
      m_envir.drawCloudNormalPass(viewMatrix, projectionMatrix); 

    });

    // second pass (lighting)
    m_lBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 0));

      m_envir.drawLightingPass(viewMatrix, projectionMatrix, gCont);
      m_mapView.drawLightingPass(viewMatrix, projectionMatrix, gCont);
    });

    // third pass (final)
    m_fBuffer.prime([&](HotFrameBuffer& hotFB) {
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));

      // TODO: remove
      hotFB.clearDepth(1.f);

      m_envir.draw(viewMatrix, projectionMatrix);

      m_mapView.drawFinalPass(viewMatrix, projectionMatrix, m_lBufferTex, m_gBufferDepth);
      m_envir.drawCloudFinalPass(viewMatrix,projectionMatrix, m_lBufferTex, m_gBufferDepth);
      m_playerView.drawFinalPass(viewMatrix, projectionMatrix, m_camera, m_lBufferTex, m_cheatmode);
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

      pMenu.prime([&](HotProgram& hotP)
      {

        hotP.uniform["s_menupng"] = 0;
        hotP.uniform["s_resSh"] = 5;
        hotP.uniform["s_Sh"] = m_button;
        hotP.uniform["s_number"] = 1;

        TexCont cont;
        cont.addTexture(0, m_resTex);
        cont.addTexture(1, m_loadTex);
        cont.addTexture(2, m_saveTex);
        cont.addTexture(3, m_optionsTex);
        cont.addTexture(4, m_exitTex);
        cont.addTexture(5, m_resSh);


        cont.prime([&](HotTexCont& hotTexCont) 
        {

          if(m_pause)
          {

            hotP.uniform["u_offset"] = (float)0.0;

            hotP.draw(hotTexCont, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-(m_size.y/(m_size.y*(3.0)));
            hotP.uniform["s_menupng"] = 1;
            hotP.uniform["s_number"] = 2;
            hotP.draw(hotTexCont, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-(m_size.y/(m_size.y*(3.0)))*2;
            hotP.uniform["s_menupng"] = 2;
            hotP.uniform["s_number"] = 3;
            hotP.draw(hotTexCont, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-(m_size.y/(m_size.y*(3.0)))*3;
            hotP.uniform["s_menupng"] = 3;
            hotP.uniform["s_number"] = 4;
            hotP.draw(hotTexCont, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

            hotP.uniform["u_offset"] = (float)-(m_size.y/(m_size.y*(3.0)))*4;
            hotP.uniform["s_menupng"] = 4;
            hotP.uniform["s_number"] = 5;
            hotP.draw(hotTexCont, m_fullScreenQuad2, PrimitiveType::TriangleStrip);

          }

        });

      });


      m_playerView.draw();

    });

    // swap buffer
    hotContext.swapBuffer();

  }

}
