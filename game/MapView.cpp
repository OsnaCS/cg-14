#include "MapView.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "ObjectLoader.hpp"

#include <math.h>
#include <set>

MapView::MapView(Map& map, Camera& cam, Environment& envir)
: m_map(map), m_cam(cam), m_envir(envir) {

}

void MapView::init() {

  ImageBox image_box = loadPNGImage("gfx/texture.png");
  m_colorTexture.create(Vec2i(2048,2048), TexFormat::RGBA8, image_box.data());
  m_colorTexture.params.filterMode = TexFilterMode::Trilinear;
  m_colorTexture.params.useMipMaps = true;

  ImageBox imageBoxNormal = loadPNGImage("gfx/normals_small.png");
  m_normalTexture.create(Vec2i(512,512), TexFormat::RGBA8, imageBoxNormal.data());
  m_normalTexture.params.filterMode = TexFilterMode::Trilinear;
  m_normalTexture.params.useMipMaps = true;


  VShader vs;
  vs.compile(loadShaderFromFile("shader/CraftGame.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/CraftGame.fsh"));

  m_program.create(vs, fs);
  m_program.perFragProc.enableDepthTest();
  m_program.perFragProc.enableDepthWrite();
  m_program.perFragProc.setDepthFunction(DepthFunction::Less);
  m_program.primitiveProc.enableCulling();

  VShader normalVS;
  normalVS.compile(loadShaderFromFile("shader/MapViewNormalPass.vsh"));
  FShader normalFS;
  normalFS.compile(loadShaderFromFile("shader/MapViewNormalPass.fsh"));

  m_normalPass.create(normalVS, normalFS);
  m_normalPass.perFragProc.enableDepthTest();
  m_normalPass.perFragProc.enableDepthWrite();
  m_normalPass.perFragProc.setDepthFunction(DepthFunction::Less);
  m_normalPass.primitiveProc.enableCulling();

  VShader finalVS;
  finalVS.compile(loadShaderFromFile("shader/MapViewFinalPass.vsh"));
  FShader finalFS;
  finalFS.compile(loadShaderFromFile("shader/MapViewFinalPass.fsh"));

  m_finalPass.create(finalVS, finalFS);
  m_finalPass.perFragProc.enableDepthTest();
  m_finalPass.perFragProc.enableDepthWrite();
  m_finalPass.perFragProc.setDepthFunction(DepthFunction::Less);
  m_finalPass.primitiveProc.enableCulling();

  VShader lightingVS;
  lightingVS.compile(loadShaderFromFile("shader/MapViewLightingPass.vsh"));
  FShader lightingFS;
  lightingFS.compile(loadShaderFromFile("shader/MapViewLightingPass.fsh"));

  m_lightingPass.create(lightingVS, lightingFS);
  m_lightingPass.perFragProc.enableDepthTest();
  m_lightingPass.perFragProc.enableDepthWrite(false);
  m_lightingPass.perFragProc.setDepthFunction(DepthFunction::Greater);
  m_lightingPass.perFragProc.blendFuncRGB = BlendFunction::Add;
  m_lightingPass.perFragProc.blendFuncA = BlendFunction::Add;
  m_lightingPass.perFragProc.srcRGBParam = BlendParam::One;
  m_lightingPass.perFragProc.dstRGBParam = BlendParam::One;
  m_lightingPass.perFragProc.srcAParam = BlendParam::One;
  m_lightingPass.perFragProc.dstAParam = BlendParam::One;
}

void MapView::drawChunks(HotProgram& hotP, HotTexCont& hotTexCont) {

  Vec2i activeChunk = m_map.getChunkPos(m_cam.get_position());

  for(int x = activeChunk.x - 4; x <= activeChunk.x + 4; x++) {
    for(int z = activeChunk.y - 4; z <= activeChunk.y + 4; z++) {

      if(m_map.exists({x * 16, 0, z * 16})) {

        Vec2i chunkPos(x, z);

        if (isChunkVisible(chunkPos)) {
          if(m_mapView.count(chunkPos) == 0) {
            m_mapView[chunkPos].init(chunkPos, m_map);
          }

          m_mapView[chunkPos].draw(hotP, hotTexCont);
        }
      }
    }
  }
}

void MapView::drawNormalPass(Mat4f viewMat, Mat4f projMat) {

  m_normalPass.prime([&](HotProgram& hotP) {

    hotP.uniform["u_view"] = viewMat;
    hotP.uniform["u_projection"] = projMat;
    hotP.uniform["u_backPlaneDistance"] = m_cam.getBackPlaneDistance();
    hotP.uniform["normalTex"] = 0;

    // u_normals[0] = Top
    hotP.uniform["u_normals[0]"] = Vec3f(0,1,0);
    // u_normals[1] = Bottom
    hotP.uniform["u_normals[1]"] = Vec3f(0,-1,0);
    // u_normals[2] = North
    hotP.uniform["u_normals[2]"] = Vec3f(0,0,-1);
    // u_normals[3] = South
    hotP.uniform["u_normals[3]"] = Vec3f(0,0,1);
    // u_normals[4] = West
    hotP.uniform["u_normals[4]"] = Vec3f(-1,0,0);
    // u_normals[5] = East
    hotP.uniform["u_normals[5]"] = Vec3f(1,0,0);

    // u_tangents[0] = Top
    hotP.uniform["u_tangents[0]"] = Vec3f(1,0,0);
    // u_tangents[0] = Bottom
    hotP.uniform["u_tangents[1]"] = Vec3f(0,0,1);
    // u_tangents[0] = North
    hotP.uniform["u_tangents[2]"] = Vec3f(1,0,0);
    // u_tangents[0] = South
    hotP.uniform["u_tangents[3]"] = Vec3f(1,0,0);
    // u_tangents[0] = West
    hotP.uniform["u_tangents[4]"] = Vec3f(0,0,1);
    // u_tangents[0] = East
    hotP.uniform["u_tangents[5]"] = Vec3f(0,0,1);


    TexCont cont;
    cont.addTexture(0, m_normalTexture);
    cont.prime([&](HotTexCont& hotTexCont) {
      
      drawChunks(hotP, hotTexCont);
    });
  });
}

void MapView::drawLightingPass(Mat4f viewMat, Mat4f projMat, TexCont& gBuffer) {

  set<Vec3f> pointLights = m_map.getPointLights();

  m_lightingPass.prime([&](HotProgram& hotProg) {

    for (Vec3f pointLight : pointLights) {

      hotProg.uniform["normalTexture"] = 0;
      hotProg.uniform["depthTexture"] = 1;
      hotProg.uniform["u_lightIntens"] = 1.f;
      hotProg.uniform["u_lightPosition"] = pointLight;
      hotProg.uniform["u_cameraPos"] = m_cam.get_position();

      Vec3f direction = m_cam.get_direction();
      float backPlaneDistance = m_cam.getBackPlaneDistance();
      float viewAngle = m_cam.getViewAngle();
      float screenRatio = m_cam.getScreenRatio();
      Vec3f viewUp = m_cam.getViewUp();

      // vector in the backplane
      Vec3f vd = cross(direction, viewUp).normalize();
      viewUp = cross(vd, direction).normalize();
      float halfBackPlaneHeight = tan(viewAngle / 2) * backPlaneDistance;
      float halfBackPlaneWidth = screenRatio * halfBackPlaneHeight;
      Vec3f backPlaneCenter = direction.normalize() * backPlaneDistance;

      VertexSeq<Vec2f, Vec3f> backPlane;
      backPlane.create(4);

      backPlane.prime([&](HotVertexSeq<Vec2f, Vec3f>& hotV) {
        // oben rechts
        hotV.vertex[0].set(Vec2f(1, 1), backPlaneCenter + (viewUp * halfBackPlaneHeight) + (vd * halfBackPlaneWidth));
        // unten rechts
        hotV.vertex[1].set(Vec2f(1, -1), backPlaneCenter - (viewUp * halfBackPlaneHeight) + (vd * halfBackPlaneWidth));
        // oben links
        hotV.vertex[2].set(Vec2f(-1, 1), backPlaneCenter + (viewUp * halfBackPlaneHeight) - (vd * halfBackPlaneWidth));
        // unten links
        hotV.vertex[3].set(Vec2f(-1, -1), backPlaneCenter - (viewUp * halfBackPlaneHeight) - (vd * halfBackPlaneWidth));
      });

      gBuffer.prime([&](HotTexCont& hotTexCont) {
        hotProg.draw(hotTexCont, backPlane, PrimitiveType::TriangleStrip);
      });
    }
  });
}

void MapView::drawFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBuffer, Tex2D& dBuffer) {

  m_finalPass.prime([&](HotProgram& hotP) {

    hotP.uniform["u_view"] = viewMat;
    hotP.uniform["u_projection"] = projMat;
    hotP.uniform["u_winSize"] = m_cam.getWindow().getSize();
    hotP.uniform["s_lightTexture"] = 0;
    hotP.uniform["s_colorTexture"] = 1;
    hotP.uniform["s_depthTexture"] = 2;
    hotP.uniform["u_time"] = m_envir.getTime();
    hotP.uniform["u_daylength"] = m_envir.getDayLength();

    TexCont cont;
    cont.addTexture(0, lBuffer);
    cont.addTexture(1, m_colorTexture);
    cont.addTexture(2, dBuffer);
    

    cont.prime([&](HotTexCont& hotCont){
      drawChunks(hotP, hotCont);
    });
  });
}

bool MapView::isChunkVisible(Vec2i& chunkPos) {

  Vec2f lookDirection(m_cam.get_direction().x, m_cam.get_direction().z);
  lookDirection.normalize();

  Vec3i playerWorldPos = vector_cast<int>(m_cam.getPosition() + Vec3f(.5f, .5f, .5f));
  Vec2i playerChunkPos = m_map.getChunkPos(playerWorldPos - Vec3i(lookDirection.x * 32, 0, lookDirection.y * 32));

  Vec2f checkVector = chunkPos - playerChunkPos;
  checkVector.normalize();
  float alpha = acos((checkVector.x * lookDirection.x) + (checkVector.y * lookDirection.y)) * 180.0 / M_PI;
  float viewAngleDegree = m_cam.getViewAngle() * 180.0 / M_PI;

  if (alpha < viewAngleDegree + 20) {
    return true;
  }

  return false;
}

bool MapView::notifiBlockUpdate(Vec3i blockPos) {

  deleteChunkView(m_map.getChunkPos(blockPos));

  if(blockPos.x % 16 == 0) {

    deleteChunkView(m_map.getChunkPos(blockPos) + Vec2i(-1, 0));
  }

  if(blockPos.x % 16 == 15) {

    deleteChunkView(m_map.getChunkPos(blockPos) + Vec2i(1, 0));
  }

  if(blockPos.z % 16 == 0) {

    deleteChunkView(m_map.getChunkPos(blockPos) + Vec2i(0, -1));
  }

  if(blockPos.z % 16 == 15) {

    deleteChunkView(m_map.getChunkPos(blockPos) + Vec2i(0, 1));
  }

}

void MapView::deleteChunkView(Vec2i chunkPos){
  m_mapView.erase(chunkPos);
}

void MapView::resetMapView(){
  m_mapView.erase(m_mapView.begin(), m_mapView.end());
}

bool MapView::exists(Vec2i pos) {
  return m_mapView.count(pos) > 0;
}

void MapView::clearMapView(Vec2i position) {
  auto it = m_mapView.begin();
  auto pred = [&](std::pair<const Vec2i, ChunkView>& entry) {
    Vec2i temp = entry.first - position;
    
    return temp.length() > 20;
  };
  while((it = std::find_if(it, m_mapView.end(), pred)) != m_mapView.end()) {
    m_mapView.erase(it++);
  }
  
}
