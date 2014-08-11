#pragma once

#include "Chunk.hpp"
#include "BlockType.hpp"
#include <map>
#include "lumina/core/LuminaException.hpp"

using namespace std;

class Map{
public:

	/**
	* add a Chunk
	* @param pos 2D Position
	*/
	void addChunk(Vec2i pos);

	/**
	* setBlockType
	* @param pos 3D Position pos.y = hight
	* @param type BlockType
	*/
	void setBlockType(Vec3i pos, BlockType type);

	/**
	* getChunkPos
	* @param pos 3D Position pos.y = hight
	* @return Vec2i 2D Position
	*/
	Vec2i getChunkPos(Vec3i pos);

	/**
	* getBlockType
	* @param pos 3D Position pos.y = hight
	* @return BlockType of the Block on the Position
	*/
	BlockType getBlockType(Vec3i pos);

	/**
	* Save's the World
	*/
	//void saveWorld();

	/**
	*	Soll die Dateien einlesen
	*/
	//map<Vec2i, Chunk> loadWorld("WeltDatei");

private:

	// Map with all Chunks
	map<Vec2i, Chunk> m_map;

};