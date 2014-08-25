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

		default:
			return Color8A(255,0,204,255);
	}

}

Vec2f getTexCoords(BlockType type, BlockSide side){
	float m = 0.125f;

	switch(type){

				

		case BlockType::Dirt:
		return m*Vec2f(2,0);

		case BlockType::Wood:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
				return m * Vec2f(5, 1);
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(4,1);
		}
		

		case BlockType::Stone:
		return m*Vec2f(0,1);


		case BlockType::Sand:
		return m*Vec2f(2,1);
		

		case BlockType::Grass:
		switch(side){
			case BlockSide::Top: 
				return m * Vec2f(0, 0);
			case BlockSide::Bottom:
				return m*Vec2f(2,0);
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(3,0);
		}

		case BlockType::Cactus:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
				return m * Vec2f(5,0);
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(4,0);
		}

		case BlockType::Birch:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
				return m * Vec2f(5, 1);
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(3,1);
		}

		case BlockType::Spruce:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
				return m * Vec2f(5, 1);
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(6,1);
		}

		case BlockType::BirchLeaves:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(6,0);
		}

		case BlockType::PineLeaves:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(7,0);
		}

		case BlockType::SpruceLeaves:
		switch(side){
			case BlockSide::Top: 
			case BlockSide::Bottom:
			case BlockSide::North:
			case BlockSide::South:
			case BlockSide::West:
			case BlockSide::East:
				return m*Vec2f(7,1);
		}


		default: return m*Vec2f(7,7);
		
	}
}