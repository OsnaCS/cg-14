#pragma once

#include "Chunk.hpp"
#include "BlockType.hpp"
#include <map>

using namespace std;

class Map{
public:

	void addChunk(Vec2i);
	void setBlockType(Vec3i pos, BlockType type);
	BlockType getBlockType(Vec3i pos);
	Vec2i getChunkPos(Vec3i pos);

	//void saveWorld();

	//map<Vec2i, Chunk> loadWorld("WeltDatei");

private:

	map<Vec2i, Chunk> m_map;

};