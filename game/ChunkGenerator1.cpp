#include "ChunkGenerator1.hpp"
#include "SimplexNoise.hpp"

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "BiomeType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include <random>
#include <math.h>
#include <string>


using namespace std;

// niedriger Wert für Oktave ergibt hügeligere Landschaft
// Perlin Noise mit Koeffizienten > 1 multipliziert ergibt höhere Steigung
ChunkGenerator1::ChunkGenerator1() {
  // Random Seed
  srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
  m_seed = rand() % 512;
  m_biome_seed = (m_seed*m_seed)%618;
}

void ChunkGenerator1::chunkGeneration(Map& map, Vec3i spectatorPos) {

  // chunkPos ist Position des Chunks, in dem der Spectator steht in Chunkkoordinaten
  Vec2i chunkPos = map.getChunkPos(spectatorPos);

  for(int x = chunkPos.x - 2; x <= chunkPos.x + 2; x++) {
    for(int z = chunkPos.y - 2; z <= chunkPos.y + 2; z++) {
      if(!map.exists({x * 16, 0, z * 16})) {
        map.addChunk({x, z});
        Chunk chunk = map.getChunk({x, z});
        double simpBiomeNoise = SimplexNoise::noise(0.01*x, 0.01*z, m_biome_seed);
        int biomeNoise = SimplexNoise::noiseInt(0, 126, simpBiomeNoise);
        slog(biomeNoise);
          if(0 <= biomeNoise && biomeNoise < 50){
            slog("DESERT");
            chunk.setBiomeType(BiomeType::Desert);
          }
          if(50 <= biomeNoise && biomeNoise < 60){
            slog("PLAINS");
            chunk.setBiomeType(BiomeType::Plains);
          }
          if(60 <= biomeNoise && biomeNoise < 70){
            slog("FOREST");
            chunk.setBiomeType(BiomeType::Forest);
          }
          if(70 <= biomeNoise && biomeNoise <= 126){
            slog("MOUNTAINS");
            chunk.setBiomeType(BiomeType::Mountains);
          } else {chunk.setBiomeType(BiomeType::Forest);}
          
        setBiomes(map, chunk, x, z);
      }
    }
  }
}

void ChunkGenerator1::setBlockHeight(Map& map, int x, int z, int xi, int zj, int noise) {
  for(int k = 0; k < 128; k++) {
    xi = xi%16;
    zj = zj%16;
    if(xi % 16 < 0) {
      xi += 16;
    }
    if(zj % 16 < 0) {
      zj += 16;
    }


    if(k == noise) {
      map.getChunk({x,z}).setBlockType({xi, k, zj}, BlockType::Grass); 
    } else if(k <= noise && k >= noise - 3) {
      map.getChunk({x,z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
    } else if(k < (noise - 3) && k >= noise - 10) {
      map.getChunk({x,z}).setBlockType({xi, k, zj}, BlockType::Stone);
    } else {
      map.getChunk({x,z}).setBlockType({xi, k, zj}, BlockType::Air); //  Über dem Noise-Wert gibt es nur Air
    }
  }
}


void ChunkGenerator1::setBiomes(Map& m, Chunk chunk, int x, int z) {


  slog("SET BIOMES");

  double frequency = getFrequency(chunk.getBiomeType());
  slog("SET THE FREQUENCY");
  int lowerBound = getLowerBound(chunk.getBiomeType());
  int upperBound = getUpperBound(chunk.getBiomeType());

  for(int xi = x * 16; xi < (x * 16) + 16; xi++) {
    // Blöcke von oben nach unten (in Blockkoordinaten)
    for(int zj = z * 16; zj < (z * 16) + 16; zj++) {
      // Simplex Noise:
      // Berechne Werte im Intervall [-1,1] mit Simplex Noise


      double simpNoise = SimplexNoise::noise(frequency * xi, frequency * zj, m_seed);
      // Umrechnen von Intervall [-1,1] in Intervall [c,d]
      int noise = SimplexNoise::noiseInt(lowerBound, upperBound, simpNoise);

      setBlockHeight(m, x, z, xi, zj, noise);
    }
  }
}


/*
void ChunkGenerator::defineBiomeType(Map& m, Chunk chunk, int x, int z) {

  int modX = abs(x) % 3;
  int modZ = abs(z) % 3;
  // Chunk chunk = m.getChunk({x, z});

  try{
  if(modX == 0) {
    if(modZ == 0) {
      random_device rd;
      uniform_int_distribution<int> dist(0, static_cast<int>(BiomeType::COUNT));
      int number = dist(rd);
      BiomeType type = static_cast<BiomeType>(number);

      chunk.setBiomeType(type);
    }
    if(modZ == 1) {
      // Look at chunk (x, z-1) above to define BiomType
      chunk.setBiomeType(m.getChunk({x, z - 1}).getBiomeType());
    }
    if(modZ == 2) {
      // Look at chunk (x, z+1) below to define BiomType
      chunk.setBiomeType(m.getChunk({x, z + 1}).getBiomeType());
    }
  }

  if(modX == 1) {
    if(modZ == 0) {
      // Look at chunk (x-1, z) to the left to define BiomType
      chunk.setBiomeType(m.getChunk({x - 1, z}).getBiomeType());
    }
    if(modZ == 1) {
      // Look at chunk (x-1, z-1) above to the left to define BiomType
      chunk.setBiomeType(m.getChunk({x - 1, z - 1}).getBiomeType());
    }
    if(modZ == 2) {
      // Look at chunk (x-1, z+1) below to the left to define BiomType
      chunk.setBiomeType(m.getChunk({x - 1, z + 1}).getBiomeType());
    }
  }

  if(modX == 2) {
    if(modZ == 0) {
      // Look at chunk (x+1, z) to the right to define BiomType
      chunk.setBiomeType(m.getChunk({x + 1, z}).getBiomeType());
    }
    if(modZ == 1) {
      // Look at chunk (x+1, z-1) above to the right to define BiomType
      chunk.setBiomeType(m.getChunk({x + 1, z - 1}).getBiomeType());
    }
    if(modZ == 2) {
      // Look at chunk (x+1, z+1) below to the right to define BiomType
      chunk.setBiomeType(m.getChunk({x + 1, z + 1}).getBiomeType());
    }
  }else{
    chunk.setBiomeType(BiomeType::Forest);
  }
}catch(OutOfRangeEx e){}
}
*/