#pragma once

#include "Chunk.hpp"
#include "BlockType.hpp"
#include <map>

using namespace std;
/**
* A Map consists of arbitrary many Chunks.
* Chunks have to be added to the Map with the function
* addChunk(Vec2i pos).
*
*/
class Map
{
	
public:

	/**
	* adds a Chunk to the Map at the given 2D Position.
	* If the Chunk already exists, it will be overwritten with
	* a new Chunk consisting only of Air.
	* To get the 2D Position of the Chunk from a 
	* 3D Position in the Map use the getChunkPos(Vec3i pos) method
	*
	* @param pos 2D Position of the Chunk
	*/
	void addChunk(Vec2i pos);

	/**
	* sets the BlockType of the given Position to the given
	* Blocktype. To check if the BlockType can be set,
	* use the exists(Vec3i pos) method
	*
	* @param pos 3D Position of the Block in the complete Map
	* 				pos.y = hight
	* @param type BlockType
	*/
	void setBlockType(Vec3i pos, BlockType type);

	/**
	* convinient Method to get the corresponding 2d Position of 
	* a Chunk to the given 3D Position of a Block
	*
	* @param pos 3D Position of the Block in the complete Map
	*					pos.y = hight
	* @return Vec2i 2D Position of the Chunk
	*/
	Vec2i getChunkPos(Vec3i pos);

	/**
	* returns the BlockType of the given Position
	*
	* @param pos 3D Position of the Block in the complete Map
	*					pos.y = hight
	* @return BlockType of the Block on the given Position
	*/
	BlockType getBlockType(Vec3i pos);


	/**
	* returns if a Block exists for the given Position
	*
	* @param pos 3D Position of the Block in the complete Map
	* @return true if the Block exists
	*/
	bool exists(Vec3i pos);


	/**
	* Save's the World
	*/
	//void saveWorld();

	/**
	*	Soll die Dateien einlesen
	*/
	//map<Vec2i, Chunk> loadWorld("WeltDatei");

	Chunk& getChunk(Vec2i pos);

private:

	// Map with all Chunks
	map<Vec2i, Chunk> m_map;

};