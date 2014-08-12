#pragma once

#include "lumina/lumina.hpp"

using namespace lumina;

/**
* Enumerates the differnet BlockTypes
*/
enum class BlockType
{

	Air, Dirt, Wood, Stone, Water, Sand, Grass
	
};

/**
* returns the Color of the given Blocktype
* @param type Blocktype of which the Color is returned
* @return the Color of the BlockType
*/
Color8A getColor(BlockType type);