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
          if(0 <= biomeNoise && biomeNoise < 45){
            chunk.setBiomeType(BiomeType::Desert);
          }
          if(45 <= biomeNoise && biomeNoise < 47){
            chunk.setBiomeType(BiomeType::DesertPlain);
          }
          if(47 <= biomeNoise && biomeNoise < 55){
            chunk.setBiomeType(BiomeType::Plains);
          }
          if(55 <= biomeNoise && biomeNoise < 57){
            chunk.setBiomeType(BiomeType::PlainForest);
          }
          if(57 <= biomeNoise && biomeNoise < 70){
            chunk.setBiomeType(BiomeType::Forest);
          }
          if(70 <= biomeNoise && biomeNoise < 72){
            chunk.setBiomeType(BiomeType::Hillside);
          }
          if(72 <= biomeNoise && biomeNoise <= 126){
            chunk.setBiomeType(BiomeType::Mountains);
          }
        setBiomes(map, chunk, x, z);
      }
    }
  }
}

void ChunkGenerator1::setBiomes(Map& m, Chunk chunk, int x, int z) {
  BiomeType type = chunk.getBiomeType();

  double frequency = getFrequency(type);
  int lowerBound = getLowerBound(type);
  int upperBound = getUpperBound(type);

  for(int xi = x * 16; xi < (x * 16) + 16; xi++) {
    // Blöcke von oben nach unten (in Blockkoordinaten)
    for(int zj = z * 16; zj < (z * 16) + 16; zj++) {
      // Simplex Noise:
      // Berechne Werte im Intervall [-1,1] mit Simplex Noise

      double simpNoise = SimplexNoise::noise(frequency * xi, frequency * zj, m_seed);
      // Umrechnen von Intervall [-1,1] in Intervall [c,d]
      int noise = SimplexNoise::noiseInt(lowerBound, upperBound, simpNoise);

      setBlockHeight(m, type, x, z, xi, zj, noise);
    }
  }
}

void ChunkGenerator1::setBlockHeight(Map& map, BiomeType type, int x, int z, int xi, int zj, int noise) {
  for(int k = 0; k < 128; k++) {
    xi = xi % 16;
    zj = zj % 16;
    if(xi % 16 < 0) {
      xi += 16;
    }
    if(zj % 16 < 0) {
      zj += 16;
    }

    switch(type) {
      case BiomeType::Desert:
        if(k == noise) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::DesertPlain:
        if(k == noise) {
          srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
          int random = rand() % 512;
          if(random < 267){
            map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand);
          }else map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

        case BiomeType::Plains:
        if(k == noise) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::PlainForest:
        if(k == noise) {
          // srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
          // int random = rand() % 512;
          // if(random < 100){ //Bäume setzen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Wood);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        }
        break;

        case BiomeType::Forest:
        if(k == noise) {
          // srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
          // int random = rand() % 512;
          // if(random < 200){ //Bäume setzen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Wood);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::Hillside:
        if(k == noise && noise <= 80) {
          srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
          int random = rand() % 512;
          if(random < 350){ 
            map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
          } else map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
        } else if (k == noise && noise > 80){
          map.getChunk({x,z}).setBlockType({xi,k,zj}, BlockType::Stone);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

        case BiomeType::Mountains:
        if(k == noise && noise > 80) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
        } else if(k == noise && noise <= 80){
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;
      default: break;
    }
    
    //untere Ebenen werden gleich generiert
    if(k < (noise - 3) && k >= noise - 10) {
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
    } else if (k > noise || k < noise-10){
      map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Air); //  Über dem Noise-Wert gibt es nur Air
    }

  }
}


