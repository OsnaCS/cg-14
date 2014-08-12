#include "ChunkGenerator.hpp"

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include <random>
#include <math.h>


using namespace std;


ChunkGenerator::ChunkGenerator(){
	m_seed = 42;
	m_octave = 2;
	//randomdings
	mt19937 rng(m_seed);
}

void ChunkGenerator::chunkGenerationPrime(Map m) {

  // generiere die 9 Startchunks, der Chunk 0,0 ist in der Mitte (Start bei
  // -1,-1)
  for (int x = -1; x < 2;
       x++) { // Chunks von links nach rechts (in Chunkkoordinaten)
    for (int z = -1; z < 2;
         z++) { // Chunks von oben nach unten (in Chunkkoordinaten) (von oben
                // nach unten wird zuerst abgearbeitet)
      m.addChunk({x, z}); // Chunk hinzufügen zur Map

      // Bearbeiten des soeben hinzugefügten Chunks
      // Blöcke von links nach rechts (in Blockkoordinaten)
      for (int i = x * 15; i < (x * 15) + 16; i++) {
        // Blöcke von oben nach unten (in Blockkoordinaten)
        for (int j = z * 15; j < (z * 15) + 16; j++) {
          int noise = perlinNoise(i, j); // Perlin Noise berechnen
          // auffüllen:
          for (int k = 0; k < 128; k++) {
            if (k <= noise) {
              m.setBlockType(
                  {i, k, j},
                  BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
            } else {
              m.setBlockType(
                  {i, k, j},
                  BlockType::Air); //  Über dem Noise-Wert gibt es nur Air
            }
          }
        }
      }
    }
        }
}

void ChunkGenerator::chunkGeneration(Map m, Vec3i player_pos){



}


int ChunkGenerator::perlinNoise(int x, int z){
	
	int x_min = x; //durch Oktave teilen?
	int x_max = x+1; 
	int z_min = z; 
	int z_max = z+1;

	Vec2i a = {x_min, z_min};
	Vec2i b = {x_max, z_min};
	Vec2i c = {x_min, z_max};
	Vec2i d = {x_max, z_max};

	float random_a = randomPos(a);
	float random_b = randomPos(b);
	float random_c = randomPos(c);
	float random_d = randomPos(d);

	int interpolated = (int)((random_a+ random_b+ random_c+ random_d)/4);

	return interpolated;
}

float ChunkGenerator::randomPos(Vec2i vector){
	float var = 10000*(sin(vector.x)+cos(vector.y)+tan(m_seed)); //randomzahl
	uniform_real_distribution<float> dist(30.f, 88.f);
	minstd_rand0 nrng(var);
	float wert = dist(nrng);
	return wert;
}




