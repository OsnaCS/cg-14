#pragma once

#include "lumina/lumina.hpp"

using namespace lumina;

/**
* Enumerates the differnet BlockTypes
*/
enum class BlockType
{
	Air, Dirt, Wood, Stone, Water, Sand, Grass, Cactus, Birch, Spruce, BirchLeaves, SpruceLeaves, PineLeaves, Torch
};

// North := back side
// South := front side
enum class BlockSide{
	Top, Bottom, North, South, West, East
};
/**
* returns the Color of the given Blocktype
* @param type Blocktype of which the Color is returned
* @return the Color of the BlockType
*/
Color8A getColor(BlockType type);

int getTexCoords(BlockType type, BlockSide side);
Vec2f getInventoryCoords(BlockType type, BlockSide side);
