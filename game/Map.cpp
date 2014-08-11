#include "Map.hpp"
#include "BlockType.hpp"

void Map::addChunk(Vec2i pos)
{

	Chunk c = Chunk();

 if (m_map.count(pos)>0)
    cout << " is already an element of m_map.\n";

	m_map[pos] = c;

}

void Map::setBlockType(Vec3i pos, BlockType type)
{

	pos.x = pos.x % 16;
	pos.z = pos.z % 16; 

	m_map.at(getChunkPos(pos)).setBlockType(pos, type);

}

Vec2i Map::getChunkPos(Vec3i pos)
{

	Vec2i pos2d;
	pos2d.x = pos.x/16;
	pos2d.y = pos.z/16;

	return pos2d;

}

BlockType Map::getBlockType(Vec3i pos)
{

	pos.x = pos.x % 16;
	pos.z = pos.z % 16; 

	return m_map.at(getChunkPos(pos)).getBlockType(pos);

}