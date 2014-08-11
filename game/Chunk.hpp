#pragma once

#include <vector>
#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "lumina/core/LuminaException.hpp"

using namespace lum;
using namespace std;

class Chunk
{

public:

	/**
	* Constructor
	*/
	Chunk();

	/**
	* setBlockType
	* @param pos 3D Position within Chunk pos.y = hight
	* @param type BlockType
	*/
	void setBlockType(Vec3i pos, BlockType type);
	
	/**
	* getBlockType
	* @param pos 3D Position within Chunk pos.y = hight
	* @return Blocktype
	*/
	BlockType getBlockType(Vec3i pos);

private:

	// Vector to save the Blocktypes
	vector<BlockType> m_blocks;
	
};