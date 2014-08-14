#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"

using namespace std;

class ChunkGenerator {
public:
  ChunkGenerator();

  // generiert alle weiteren Chunks abhängig davon wo der Player hinläuft
  void chunkGeneration(Map& m, Vec3i player_pos);

  // berechnet den Perlin Noise Wert (eine randomisierte Höhe)
  int perlinNoise(int x, int z);

	float randomPos(Vec2i vector);
  //float randomPos(int x, int z);

  float interpolate(float a, float b, float x);

private:
  long m_seed;
  int m_octave;
};
