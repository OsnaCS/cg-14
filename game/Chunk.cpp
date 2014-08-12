#include "Chunk.hpp"
#include "BlockType.hpp"

Chunk::Chunk()
 : m_blocks(16*16*128)
{

}

void Chunk::setBlockType(Vec3i pos, BlockType type)
{

	if(pos.y > 127)
	{
		throw OutOfRangeEx("y value higher than 127");
	}
	if(pos.y < 0)
	{
		throw OutOfRangeEx("y value lower than 0");
	}

	m_blocks.at(pos.y*256 + pos.z*16 + pos.x) = type;
	
	// std::cout << pos.y*256 + pos.z*16 + pos.x << std::endl;
	// std::cout << static_cast<int>(m_blocks[pos.y*256 + pos.z*16 + pos.x]) << std::endl;

}

BlockType Chunk::getBlockType(Vec3i pos)
{

	// y ist höhe 
	if(pos.y > 127)
	{
		throw OutOfRangeEx("y value higher than 127");
	}
	if(pos.y < 0)
	{
		throw OutOfRangeEx("y value lower than 0");
	}

	// std::cout << pos.y*256 + pos.z*16 + pos.x << std::endl;
	// std::cout << static_cast<int>(m_blocks[pos.y*256 + pos.z*16 + pos.x]) << std::endl;

	return m_blocks[pos.y*256 + pos.z*16 + pos.x];

}