#include "MapView.hpp"

MapView::MapView(Map& map, Camera& cam)
: m_map(map), m_cam(cam) {

}

void MapView::draw(HotProgram& hotProg) {

  Vec2i activeChunk = m_map.getChunkPos(m_cam.get_position());

  for(int x = activeChunk.x - 4; x <= activeChunk.x + 4; x++) {
    for(int z = activeChunk.y - 4; z <= activeChunk.y + 4; z++) {

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

  //float viewAngle = m_cam.getViewAngle();
  Vec2f lookDirection(m_cam.get_direction().x, m_cam.get_direction().z);
  Vec3i playerWorldPos = Vec3i(static_cast<int>(round(m_cam.getPosition().x)), static_cast<int>(round(m_cam.getPosition().y)), static_cast<int>(round(m_cam.getPosition().z)));
  Vec2i playerChunkPos = m_map.getChunkPos(playerWorldPos);

  Vec2i checkVector = chunkPos - playerChunkPos;
  checkVector.normalize();



  if ((checkVector.x * lookDirection.x) + (checkVector.y * lookDirection.y) >= 0) {

    return true;

  }

  // TODO: Richtig funktionieren muss es!
  return true;
}
