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

	Vec2i pos2d = getChunkPos(pos);

	pos.x = pos.x % 16;
	if(pos.x < 0)
		pos.x = pos.x + 16;

	pos.z = pos.z % 16; 
	if(pos.z < 0)
		pos.z = pos.z + 16;

	m_map.at(pos2d).setBlockType(pos, type);

}

Vec2i Map::getChunkPos(Vec3i pos)
{

	Vec2i pos2d;
	pos2d.x = pos.x/16;
	pos2d.y = pos.z/16;

	//C++ rounds negative number to 0, but we need it to be rounded down
	if(pos.x<0 && (pos.x%16 != 0))
		pos2d.x = pos2d.x - 1;
	if(pos.z<0 && (pos.z%16 != 0))
		pos2d.y = pos2d.y - 1;
	
	return pos2d;

}

BlockType Map::getBlockType(Vec3i pos)
{

	Vec2i pos2d = getChunkPos(pos);

	pos.x = pos.x % 16;
	if(pos.x < 0)
		pos.x = pos.x + 16;

	pos.z = pos.z % 16; 
	if(pos.z < 0)
		pos.z = pos.z + 16;

	return m_map.at(pos2d).getBlockType(pos);

}

bool Map::exists(Vec3i pos)
{

	return m_map.count(getChunkPos(pos)) > 0;

}

Chunk& Map::getChunk(Vec2i pos)
{


	return m_map[pos];
}