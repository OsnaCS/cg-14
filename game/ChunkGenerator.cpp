#include "ChunkGenerator.hpp"
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
ChunkGenerator::ChunkGenerator() {
  // Random Seed
  srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
  m_seed = rand() % 512;
  m_biome_seed = (m_seed*m_seed)%618;
}

void ChunkGenerator::chunkGeneration(Map& map, Vec3i spectatorPos, MapView& mapView) {

  // chunkPos ist Position des Chunks, in dem der Spectator steht in Chunkkoordinaten
  Vec2i chunkPos = map.getChunkPos(spectatorPos);

  for(int x = chunkPos.x - 8; x <= chunkPos.x + 8; x++) {
    for(int z = chunkPos.y - 8; z <= chunkPos.y + 8; z++) {

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
        if(70 <= biomeNoise && biomeNoise < 75){
          chunk.setBiomeType(BiomeType::Hillside);
        }
        if(75 <= biomeNoise && biomeNoise <= 126){
          chunk.setBiomeType(BiomeType::Mountains);
        }

        setBiomes(map, chunk, x, z, biomeNoise);
        
        Vec2i chuPos = Vec2i(x + 1, z);
        if(mapView.exists(chuPos)) {
          mapView.deleteChunkView(chuPos);
        }
        chuPos = Vec2i(x - 1, z);
        if(mapView.exists(chuPos)) {
          mapView.deleteChunkView(chuPos);
        }
        chuPos = Vec2i(x, z + 1);
        if(mapView.exists(chuPos)) {
          mapView.deleteChunkView(chuPos);
        }
        chuPos = Vec2i(x, z - 1);
        if(mapView.exists(chuPos)) {
          mapView.deleteChunkView(chuPos);
        }
      }
    }
  }
}

void ChunkGenerator::setBiomes(Map& m, Chunk chunk, int x, int z, int biomeNoise) {
  BiomeType type = chunk.getBiomeType();

  double frequency = getFrequency(type);
  int lowerBound = getLowerBound(type);
  int upperBound = getUpperBound(type);

  for(int xi = x * 16; xi < (x * 16) + 16; xi++) {
    // Blöcke von oben nach unten (in Blockkoordinaten)
    for(int zj = z * 16; zj < (z * 16) + 16; zj++) {




      // Simplex Noise:
      double simpNoise;
      int noise;
      // Wenn Biom Mountains oder Hillside
      if(70 <= biomeNoise && biomeNoise <= 126){
        // Umrechnen von Intervall [70,126] in [-1.25,1.25] für Gaußsche Glockenkurve
        double term = ((1.75 * biomeNoise - (-1.75) * biomeNoise + 126 * (-1.75) - 1.75 * 70) / (126.0 - 70.0));
        // Multiplikation von Simplex Noise mit Gaußscher Glockenkurve für einen weicheren Biomübergang an den Bergen
        simpNoise = exp(-1 * term * term) * SimplexNoise::noise(frequency * xi, frequency * zj, m_seed);
        // Umrechnen von Intervall [-1,1] in Intervall [c,d]
        noise = SimplexNoise::noiseInt(lowerBound, upperBound, simpNoise) -7;
      }
      else {
        // Berechne Werte im Intervall [-1,1] mit Simplex Noise
        simpNoise = SimplexNoise::noise(frequency * xi, frequency * zj, m_seed);
        // Umrechnen von Intervall [-1,1] in Intervall [c,d]
        noise = SimplexNoise::noiseInt(lowerBound, upperBound, simpNoise);
      }

      // xi und zj umrechnen
      int xii = xi%16;
	    int zjj = zj%16;
	    if(xii < 0) {
	      xii += 16;
	    }
	    if(zjj % 16 < 0) {
	      zjj += 16;
	    }

      setBlockHeight(m, type, x, z, xii, zjj, noise);
    }
  }
}

void ChunkGenerator::setBlockHeight(Map& map, BiomeType type, int x, int z, int xi, int zj, int noise) {
  for(int k = 0; k < 128; k++) {
    switch(type) {
      case BiomeType::Desert:
        if(k == noise) {
        	// Bäume setzen
          int treeDist = rand() % 10000;
          if(xi >= 2 && xi <= 13 && zj >= 3 && zj <= 13) {
          	if(treeDist <= 20) {
            	setCactusTree(map, x, z, xi, zj, noise);	
          	}
      		}
      		// Boden erzeugen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::DesertPlain:
        if(k == noise) {
        	// Bäume setzen
          int treeDist = rand() % 10000;
          if(xi >= 2 && xi <= 13 && zj >= 3 && zj <= 13) {
          	if(treeDist <= 5) {
            	setCactusTree(map, x, z, xi, zj, noise);	
          	}
      		}
      		// Boden erzeugen
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
        	// Bäume setzen
          int treeDist = rand() % 10000;
          if(xi >= 6 && xi <= 9 && zj >= 6 && zj <= 9) {
          	if(treeDist <= 30) {
            	setPalmTree(map, x, z, xi, zj, noise);	
          	}
      		}
      		// Boden erzeugen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::PlainForest:
        if(k == noise) {
        	// Bäume setzen
          int treeDist = rand() % 10000;
          if(xi >= 3 && xi <= 13 && zj >= 3 && zj <= 13) {
          	if(treeDist <= 100) {
            	setSpruceTree(map, x, z, xi, zj, noise);	
          	}
      			if(31 <= treeDist && treeDist <= 60) {
      				setBirchTree(map, x, z, xi, zj, noise);	
      			}
      		}
      		// Boden erzeugen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        }
        break;

        case BiomeType::Forest:
        if(k == noise) {
          // Bäume setzen
          int treeDist = rand() % 10000;
          if(xi >= 2 && xi <= 13 && zj >= 3 && zj <= 13) {
          	if(treeDist <= 100) {
            	setSpruceTree(map, x, z, xi, zj, noise);	
          	}
      			if(101 <= treeDist && treeDist <= 200) {
      				setBirchTree(map, x, z, xi, zj, noise);	
      			}
      		}
      		// Boden erzeugen
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

      case BiomeType::Hillside:
        if(k == noise && noise <= 77) {
          srand(time(0) + clock() + random()); // Zufallsgenerator initialisieren
          int random = rand() % 512;
          if(random < 350){ 
            map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Grass);
          } else map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
        } else if (k == noise && noise > 77){
          map.getChunk({x,z}).setBlockType({xi,k,zj}, BlockType::Stone);
        } else if(k <= noise && k >= noise - 3) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
        } 
        break;

        case BiomeType::Mountains:
        if(k == noise && noise > 72) {
          map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Stone);
        } else if(k == noise && noise <= 72){
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
    }
  }
}


void ChunkGenerator::setBirchTree(Map& map, int x, int z, int xi, int zj, int noise) {
	
		// Baummstumpf
    int k = noise + 1;

    // Baummhöhe
    int height = 3;

    // Baumstamm
    for(int l = 0; l <= height; l++) {
    	map.getChunk({x, z}).setBlockType({xi, k+l, zj}, BlockType::Birch);
    }

    // 4th row
   	k = k + height;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::Birch);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj+1}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj-1}, BlockType::BirchLeaves);


  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-2}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+2}, BlockType::BirchLeaves);

  	// 3rd row
   	k++;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::Birch); // Birch
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj+1}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj-1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj-1}, BlockType::BirchLeaves);


  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-2}, BlockType::BirchLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+2}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+2}, BlockType::BirchLeaves);

  	// 2nd row
   	k++;;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::Birch); // Birch
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::BirchLeaves);

  	// 1st row
   	k++;;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::BirchLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::BirchLeaves);

  	// top row
  	k++;
		map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::BirchLeaves);
}


void ChunkGenerator::setSpruceTree(Map& map, int x, int z, int xi, int zj, int noise) {

	// Baummstumpf
    int k = noise + 1;

    // Baummhöhe
    int height = 2;

    // Baumstamm
    for(int l = 0; l <= height; l++) {
    	map.getChunk({x, z}).setBlockType({xi, k+l, zj}, BlockType::Spruce);
    }

    // 4th row
   	k = k + height;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::Spruce);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj}, BlockType::SpruceLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj+1}, BlockType::SpruceLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-2, k+1, zj-1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+2, k+1, zj-1}, BlockType::SpruceLeaves);


  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-2}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj-2}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj-2}, BlockType::SpruceLeaves);

  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+2}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj+2}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj+2}, BlockType::SpruceLeaves);

  	// 3rd row
   	k++;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::Spruce);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::SpruceLeaves);

  	// 2nd row
   	k++;;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::SpruceLeaves);

  	// 1st row
   	k++;;
   	map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi-1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi+1, k+1, zj}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj+1}, BlockType::SpruceLeaves);
  	map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::SpruceLeaves);

  	// top row
  	k++;
		map.getChunk({x, z}).setBlockType({xi, k+1, zj}, BlockType::SpruceLeaves);
}


void ChunkGenerator::setCactusTree(Map& map, int x, int z, int xi, int zj, int noise) {

	// Baummstumpf
  int k = noise + 1;

  // Baummhöhe
  int height = 7;

  // Baumstamm
  for(int l = 0; l <= height; l++) {
  	map.getChunk({x, z}).setBlockType({xi, k+l, zj}, BlockType::Cactus);
  }

  // 1st arm
  k += 1;
  map.getChunk({x, z}).setBlockType({xi+1, k, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi+2, k, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi+2, k+1, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi+2, k+2, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi+2, k+3, zj}, BlockType::Cactus);

  // 2nd arm
  map.getChunk({x, z}).setBlockType({xi, k+2, zj+1}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+2, zj+2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+3, zj+2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+4, zj+2}, BlockType::Cactus);

  // 3rd arm
  map.getChunk({x, z}).setBlockType({xi, k+1, zj-1}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+1, zj-2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+2, zj-2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+3, zj-2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+4, zj-2}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi, k+5, zj-2}, BlockType::Cactus);

  // 4th arm
  map.getChunk({x, z}).setBlockType({xi-1, k+4, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi-2, k+4, zj}, BlockType::Cactus);
  map.getChunk({x, z}).setBlockType({xi-2, k+5, zj}, BlockType::Cactus);
}


void ChunkGenerator::setPalmTree(Map& map, int x, int z, int xi, int zj, int noise) {
	// Baummstumpf
  int k = noise + 1;

  // Baummhöhe
  int height = 9;

  // Baumstamm
  for(int l = 0; l <= height; l++) {
  	map.getChunk({x, z}).setBlockType({xi, k+l, zj}, BlockType::Wood);
  }  

  // 3th row
  k = k + height - 1;
  map.getChunk({x, z}).setBlockType({xi-4, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-5, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-6, k, zj}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi+4, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+5, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+6, k, zj}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi-4, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-5, k, zj-1}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi-4, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-5, k, zj+1}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi+4, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+5, k, zj-1}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi+4, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+5, k, zj+1}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj-4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi, k, zj-5}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi, k, zj-6}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj+4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi, k, zj+5}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi, k, zj+6}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi-1, k, zj+4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj+5}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi+1, k, zj+4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj+5}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi-1, k, zj-4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj-5}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi+1, k, zj-4}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj-5}, BlockType::Sand);

  // Coconuts
  map.getChunk({x, z}).setBlockType({xi+1, k, zj}, BlockType::Stone);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj}, BlockType::Stone);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj-1}, BlockType::Stone);
  map.getChunk({x, z}).setBlockType({xi, k, zj+1}, BlockType::Stone);


  // 2nd row
  k++;
  map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Wood);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-2, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-3, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+2, k, zj}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+3, k, zj}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-2, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-3, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+2, k, zj-1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+3, k, zj-1}, BlockType::Sand);

	map.getChunk({x, z}).setBlockType({xi, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-2, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-3, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+2, k, zj+1}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+3, k, zj+1}, BlockType::Sand);


  map.getChunk({x, z}).setBlockType({xi, k, zj-2}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj-2}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj-2}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj+2}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj+2}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj+2}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj-3}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj-3}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj-3}, BlockType::Sand);

  map.getChunk({x, z}).setBlockType({xi, k, zj+3}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi-1, k, zj+3}, BlockType::Sand);
  map.getChunk({x, z}).setBlockType({xi+1, k, zj+3}, BlockType::Sand);

  // 1st row
  k++;
  map.getChunk({x, z}).setBlockType({xi, k, zj}, BlockType::Sand);
}