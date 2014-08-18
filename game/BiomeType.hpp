#pragma once

#include "lumina/lumina.hpp"

using namespace lumina;

/**
* Enumerates the differnet BlockTypes
*/
enum class BiomeType
{
	Plains, Forest, Desert, Mountains,


	COUNT
};

double getFrequency(BiomeType type);

int getLowerBound(BiomeType type);

int getUpperBound(BiomeType type);

