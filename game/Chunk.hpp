#pragma once

#include <vector>
#include "lumina/lumina.hpp"
#include "BlockType.hpp"

using namespace lum;
using namespace std;
/**
* A Chunk consists of 16*128*16 Blocks.
* A Chunk has the height of 128 Blocks. The hight corresponds to the y-Axis in the Position Vectors.
* Position range for X-Axis=0-15, for Y-Axis=0-127, for Z-Axis=0-15.
*
*/
class Chunk
{

public:

	/**
	* Creates a Chunk consisting only of Air.
	*/
	Chunk();

	/**
	* Sets the BlockType of the given Position to the given BlockType
	* @param pos 3D Position within Chunk
	*					pos.y = hight
	* @param type BlockType
	*/
	void setBlockType(Vec3i pos, BlockType type);
	
	/**
	* getBlockType
	* @param pos 3D Position within Chunk 
	*					pos.y = hight
	* @return Blocktype
	*/
	BlockType getBlockType(Vec3i pos);



	// Vector to save the Blocktypes
	vector<BlockType> m_blocks;

	private:
	
};