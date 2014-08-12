#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"


using namespace std;

class ChunkGenerator {
public:
  ChunkGenerator();

  // generiert den Chunk auf dem der Player startet
  void chunkGenerationPrime(Map& m);

  // generiert alle weiteren Chunks abhängig davon wo der Player hinläuft
  void chunkGeneration(Map m, Vec3i player_pos);

  // berechnet den Perlin Noise Wert (eine randomisierte Höhe)
  int perlinNoise(int x, int z);

  float randomPos(Vec2i vector);

private:
  long m_seed;
  int m_octave;
  // random
};
