#include "Map.hpp"
#include "BlockType.hpp"

Vec2i Map::getChunkPos(Vec3i pos)
{

	Vec2i pos2d;
	pos2d.x = pos.x/16;
	pos2d.y = pos.z/16;

	return pos2d;
}

BlockType Map::getBlockType(Vec3i pos)
{
	
	Chunk c = m_map.at(getChunkPos(pos));	

	pos.x = pos.x % 16;
	pos.z = pos.z % 16; 

	return c.getBlockType(pos);
}

void Map::setBlockType(Vec3i pos, BlockType type)
{

	Chunk c = m_map.at(getChunkPos(pos));	

	pos.x = pos.x % 16;
	pos.z = pos.z % 16; 

	c.setBlockType(pos, type);

}

void Map::addChunk(Vec2i pos)
{
	Chunk c = Chunk();
	//map count pos
	m_map[pos] = c;
}