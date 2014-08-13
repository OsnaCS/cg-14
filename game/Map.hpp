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

	Map();

	Map(string name);

	/**
	* sets the name of the Map
	*
	* @param name new name of the Map
	*/
	void setName(string name);

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
	* returns the Chunk of the given 2D Position
	*
	* @param pos 2D Position of the Chunk
	*
	* @return 
	*/
	Chunk& getChunk(Vec2i pos);

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
	void saveWorld();

	/**
	*	Soll die Dateien einlesen
	*/
	void loadWorld(string chunkname);

private:

	// Map with all Chunks
	map<Vec2i, Chunk> m_map;

	//Name of the Map
	string m_name;

};