#pragma once

#include <vector>
#include "lumina/lumina.hpp"
#include "BlockType.hpp"

using namespace lum;
using namespace std;

class Chunk{

public:

	Chunk();
	
	BlockType getBlockType(Vec3i pos);
	void setBlockType(Vec3i pos, BlockType type);

private:

	vector<BlockType> m_blocks;
};