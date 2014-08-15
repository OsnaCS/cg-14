#pragma once

#include <map>

#include "ChunkView.hpp"
#include "Map.hpp"
#include "Camera.hpp"

class MapView
{

public:

	MapView(Map& map, Camera& cam);
	void draw(HotProgram& hotProg);
	void init();

	/**
	* Checks if a chunk is visible
	*/
	bool isChunkVisible(Vec2i& chunkPos);

private:

	map<Vec2i, ChunkView> m_mapView;
	Map& m_map;
	Camera& m_cam;
	Tex2D m_colorTexture;
};