#include "ChunkGenerator.hpp"
#include "SimplexNoise.hpp"

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include <random>
#include <math.h>


using namespace std;

// niedriger Wert für Oktave ergibt hügeligere Landschaft
// Perlin Noise mit Koeffizienten > 1 multipliziert ergibt höhere Steigung
ChunkGenerator::ChunkGenerator() {
  // Random Seed
  srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
  m_seed = rand() % 512;
}

void ChunkGenerator::chunkGeneration(Map& m, Vec3i spectator_pos) {
  Vec2i chunkPos = m.getChunkPos(spectator_pos);
  for(int x = chunkPos.x - 6; x <= chunkPos.x + 6; x++) {
    for(int z = chunkPos.y - 6; z <= chunkPos.y + 6; z++) {
      if(!m.exists({x * 16, 0, z * 16})) {
        m.addChunk({x, z});
        for(int xi = x * 16; xi < (x * 16) + 16; xi++) {
          // Blöcke von oben nach unten (in Blockkoordinaten)
          for(int zj = z * 16; zj < (z * 16) + 16; zj++) {
            // Simplex Noise:
            // Berechne Werte im Intervall [-1,1] mit Simplex Noise
            double m_frequency = 0.01;
            double simpNoise = SimplexNoise::noise(m_frequency * xi, m_frequency * zj, m_seed);
            // Umrechnen von Intervall [-1,1] in Intervall [c,d]
            int noise = SimplexNoise::noiseInt(72, 79, simpNoise);

            setBlockHeight(m, x, z, xi, zj, noise);
          }
        }
      }
    }
  }
}

void ChunkGenerator::setBlockHeight(Map& map, int x, int z, int xi, int zj, int noise) {
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
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);  
    } else if(k <= noise && k >= noise - 3) {
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter demao Noise-Wert gibt es nur Dirt
    } else if(k < (noise - 3) && k >= noise - 10) {
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
    } else {
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Air); //  Über dem Noise-Wert gibt es nur Aip
    }
  }
}
