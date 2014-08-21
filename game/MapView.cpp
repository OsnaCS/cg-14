#include "MapView.hpp"
#include "lumina/io/ImageJPEG.hpp"

#include <math.h>

MapView::MapView(Map& map, Camera& cam)
: m_map(map), m_cam(cam) {

}

void MapView::init() {

  ImageBox image_box = loadJPEGImage("gfx/texture_small.jpg");
  m_colorTexture.create(Vec2i(512,512), TexFormat::RGB8, image_box.data());
  m_colorTexture.params.filterMode = TexFilterMode::Trilinear;
  m_colorTexture.params.useMipMaps = true;

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

  ImageBox imageBoxNormal = loadJPEGImage("gfx/normals_small.jpg");
  m_normalTexture.create(Vec2i(512,512), TexFormat::RGB8, imageBoxNormal.data());
  m_normalTexture.params.filterMode = TexFilterMode::Trilinear;
  m_normalTexture.params.useMipMaps = true;
}

void MapView::drawChunks(HotProgram& hotP, HotTexCont& hotTexCont) {

  Vec2i activeChunk = m_map.getChunkPos(m_cam.get_position());

  for(int x = activeChunk.x - 6; x <= activeChunk.x + 6; x++) {
    for(int z = activeChunk.y - 6; z <= activeChunk.y + 6; z++) {

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

    TexCont cont;
    cont.addTexture(0, m_normalTexture);
    cont.prime([&](HotTexCont& hotTexCont) {
      drawChunks(hotP, hotTexCont);
    });
  });
}

void MapView::drawFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBuffer) {

  m_finalPass.prime([&](HotProgram& hotP) {

    hotP.uniform["u_view"] = viewMat;
    hotP.uniform["u_projection"] = projMat;
    hotP.uniform["u_winSize"] = m_cam.getWindow().getSize();
    hotP.uniform["s_lightTexture"] = 0;
    hotP.uniform["s_colorTexture"] = 1;
    
    lBuffer.prime(0, [&](HotTex2D& hotLightingTex) {
      m_colorTexture.prime(1, [&](HotTex2D& hotTex) {

        HotTexCont hotTexCont(hotLightingTex, hotTex);
        drawChunks(hotP, hotTexCont);
      });
    });
  });
}

bool MapView::isChunkVisible(Vec2i& chunkPos) {

  Vec2f lookDirection(m_cam.get_direction().x, m_cam.get_direction().z);
  lookDirection.normalize();
  Vec2i offsetDirection(round(lookDirection.x), round(lookDirection.y));
  Vec3i playerWorldPos = Vec3i(static_cast<int>(round(m_cam.getPosition().x)), static_cast<int>(round(m_cam.getPosition().y)), static_cast<int>(round(m_cam.getPosition().z)));
  Vec2i playerChunkPos = m_map.getChunkPos(playerWorldPos);

  // TODO: Winkel hat zu wenig "Spiel"
  Vec2f checkVector = chunkPos - (playerChunkPos - offsetDirection);
  checkVector.normalize();
  float alpha = acos((checkVector.x * lookDirection.x) + (checkVector.y * lookDirection.y)) * 180.0 / M_PI;
  float viewAngleDegree = m_cam.getViewAngle() * 180.0 / M_PI;

  if (alpha < viewAngleDegree + 20) {
    return true;
  }

  return false;
}
