#include "Chunk.hpp"
#include "BlockType.hpp"
#include "BiomeType.hpp"

Chunk::Chunk()
 : m_blocks(16*16*128)
{
	
}

void Chunk::setBlockType(Vec3i pos, BlockType type)
{

	if(pos.y > 127)
	{
		throw OutOfRangeEx("[Chunk] y value higher than 127");
	}
	if(pos.y < 0)
	{
		throw OutOfRangeEx("[Chunk] y value lower than 0");
	}

	m_blocks.at(pos.y*256 + pos.z*16 + pos.x) = type;

}

BlockType Chunk::getBlockType(Vec3i pos)
{

	// y ist Höhe 
	if(pos.y > 127)
	{
		throw OutOfRangeEx("[Chunk] y value higher than 127");
	}
	if(pos.y < 0)
	{
		throw OutOfRangeEx("[Chunk] y value lower than 0");
	}

	return m_blocks[pos.y*256 + pos.z*16 + pos.x];

}

void Chunk::setBiomeType(BiomeType type){
  chunkBiomeType = type;
}

BiomeType Chunk::getBiomeType(){
  return chunkBiomeType;
}







