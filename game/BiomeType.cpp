#include "BiomeType.hpp"
#include "lumina/lumina.hpp"

double getFrequency(BiomeType type) {
  switch(type) {
    case BiomeType::Desert:
      return 0.01;
    case BiomeType::DesertPlain:
      return 0.0075;
    case BiomeType::Plains:
      return 0.005;
    case BiomeType::PlainForest:
      return 0.0125;
    case BiomeType::Forest:
      return 0.02;
    case BiomeType::Hillside:
      return 0.025;
    case BiomeType::Mountains:
      return 0.03;

    default:
      slog("Unknown BiomeType in getFrequency");
  }
}


int getLowerBound(BiomeType type) { return 72; }

int getUpperBound(BiomeType type) {

  switch(type) {
    case BiomeType::Desert:
      return 76;
    case BiomeType::DesertPlain:
      return 74;
    case BiomeType::Plains:
      return 74;
    case BiomeType::PlainForest:
      return 76;
    case BiomeType::Forest:
      return 78;
    case BiomeType::Hillside:
      return 82;
    case BiomeType::Mountains:
      return 93;
    default:
      slog("Unknown BiomeType in getUpperBound");
  }
}