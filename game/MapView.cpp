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

	    	if(m_mapView.count(chunkPos) == 0) {
	    		m_mapView[chunkPos].init(chunkPos, m_map);
	    	}

	    	m_mapView[chunkPos].draw(hotProg);
    	}
    }
  }
}