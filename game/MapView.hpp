#pragma once

#include <map>

#include "ChunkView.hpp"

class MapView
{
	
public:

	MapView();
	void addChunkView(Vec2i pos);
	ChunkView& getChunkView(Vec2i pos);

private: 
	// Map with all Chunks
	map<Vec2i, Chunk> m_map;

	//Name of the Map
	string m_name;

};