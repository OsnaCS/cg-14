#include "BiomeType.hpp"
#include "lumina/lumina.hpp"

double getFrequency(BiomeType type){
switch(type){
	case BiomeType::Plains:
	  return 0.001;

	case BiomeType::Desert:
	  return 0.01;

	case BiomeType::Forest:
	  return 0.02;

	case BiomeType::Mountains:
	  return 0.06;

	default: slog("Unknown BiomeType in getFrequency");
}
}


int getLowerBound(BiomeType type){

return 72;

}


int getUpperBound(BiomeType type){

switch(type){
	case BiomeType::Plains:
	  return 74;

	case BiomeType::Desert:
	  return 76;

	case BiomeType::Forest:
	  return 78;

	case BiomeType::Mountains:
	  return 90;

	default: slog("Unknown BiomeType in getUpperBound");
}

}