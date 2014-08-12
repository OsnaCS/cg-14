#include "BlockType.hpp"

Color8A getColor(BlockType type)
{

	switch(type)
	{
		case BlockType::Air:
			return Color8A(255,255,255,0);

		case BlockType::Dirt:
			return Color8A(205,175,149,255);

		case BlockType::Wood:
			return Color8A(139,69,19,255);

		case BlockType::Stone:
			return Color8A(105,105,105,255);

		case BlockType::Water:
			return Color8A(24,116,205,127);

		case BlockType::Sand:
			return Color8A(255,228,196,255);

		case BlockType::Grass:
			return Color8A(102,205,0,255);			
	}

}