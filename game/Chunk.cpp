#include "Chunk.hpp"
#include "BlockType.hpp"

Chunk::Chunk()
 : m_blocks(16*16*128)
{

}

BlockType Chunk::getBlockType(Vec3i pos)
{
	// z ist höhe 
	return m_blocks[pos.y*256 + pos.z*16 + pos.x];
}

void Chunk::setBlockType(Vec3i pos, BlockType type)
{

	m_blocks.at(pos.y*256 + pos.z*16 + pos.x) = type;

}
