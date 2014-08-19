#pragma once

#include "lumina/lumina.hpp"

using namespace lumina;

enum class BiomeType
{
	Desert, DesertPlain, Plains, PlainForest, Forest, Hillside, Mountains,

	COUNT
};

double getFrequency(BiomeType type);

int getLowerBound(BiomeType type);

int getUpperBound(BiomeType type);

