#include "MapView.hpp"

#include <math.h>

MapView::MapView(Map& map, Camera& cam)
: m_map(map), m_cam(cam) {

}

void MapView::draw(HotProgram& hotProg) {

  Vec2i activeChunk = m_map.getChunkPos(m_cam.get_position());

  for(int x = activeChunk.x - 10; x <= activeChunk.x + 10; x++) {
    for(int z = activeChunk.y - 10; z <= activeChunk.y + 10; z++) {

    	if(m_map.exists({x * 16, 0, z * 16})) {

    		Vec2i chunkPos(x, z);

        if (isChunkVisible(chunkPos)) {
          if(m_mapView.count(chunkPos) == 0) {
            m_mapView[chunkPos].init(chunkPos, m_map);
          }

          m_mapView[chunkPos].draw(hotProg);
        }
      }
    }
  }
}

bool MapView::isChunkVisible(Vec2i& chunkPos) {

  Vec2f lookDirection(m_cam.get_direction().x, m_cam.get_direction().z);
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
