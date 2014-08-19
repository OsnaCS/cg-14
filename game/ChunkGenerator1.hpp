#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "BiomeType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"

using namespace std;

class ChunkGenerator1 {
public:
  ChunkGenerator1();

  // generiert alle weiteren Chunks abhängig davon wo der Player hinläuft
  void chunkGeneration(Map& m, Vec3i player_pos);


private:
  long m_seed;
  long m_biome_seed;

  /**
   * @param map Map
   * @param x x-Wert des Blockes in Weltkoordinaten
   * @param z z-Wert des Blockes in Weltkoordinaten 
   * @param xi x-Wert des Blockes in Chunckkoordinaten,
   * @param zj z-Wert des Blockes in Chunckkoordinaten
   * @param noise Höhenwert des zu setzenden Blockes
   */
  void setBlockHeight(Map& m, BiomeType type, int x, int z, int xi, int zj, int noise);

  void setBiomes(Map& map, Chunk chunk, int x, int y);

};