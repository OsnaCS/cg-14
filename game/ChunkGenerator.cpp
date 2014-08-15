#include "ChunkGenerator.hpp"
#include "SimplexNoise.hpp"

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include <random>
#include <math.h>


using namespace std;

//niedriger Wert für Oktave ergibt hügeligere Landschaft
//Perlin Noise mit Koeffizienten > 1 multipliziert ergibt höhere Steigung
ChunkGenerator::ChunkGenerator() {
  // Random Seed
  srand(time(0) + clock() + random());  // Zufallsgenerator initialisieren 
  m_seed = rand() % 512;

  m_octave = 3;
  mt19937 rng(m_seed);
}

void ChunkGenerator::chunkGeneration(Map& m, Vec3i spectator_pos) {
  Vec2i chunkPos = m.getChunkPos(spectator_pos);
  for(int x = chunkPos.x - 4; x <= chunkPos.x + 4; x++) {
    for(int z = chunkPos.y - 4; z <= chunkPos.y + 4; z++) {
      if(!m.exists({x * 16, 0, z * 16})) {
        m.addChunk({x, z});
        for(int i = x * 16; i < (x * 16) + 16; i++) {
          // Blöcke von oben nach unten (in Blockkoordinaten)
          for(int j = z * 16; j < (z * 16) + 16; j++) {
            
            // // Perlin Noise:
            // int noise = perlinNoise(i, j); 

            // Simplex Noise:
            // Berechne Werte im Intervall [-1,1] mit Simplex Noise
            double m_frequency = 0.01;
            double simpNoise = SimplexNoise::noise(m_frequency * i, m_frequency * j, m_seed);
            // Umrechnen von Intervall [a,b] in Intervall [c,d]
            int m_a = -1;
            int m_b = 1;
            int m_c = 71;
            int m_d = 89;
            simpNoise = ((m_d * simpNoise - m_c * simpNoise + m_b * m_c - m_d * m_a) / (m_b - m_a));
            
            // Caste simpNoise als Integer
            int noise = (int) (simpNoise + 0.5);

            // auffüllen:
            for(int k = 0; k < 128; k++) {
              if(k == noise) {
                m.getChunk({x, z}).setBlockType({abs(i % 16), k, abs(j % 16)}, BlockType::Grass);
              } else if(k <= noise && k >= noise - 3) {
                m.getChunk({x, z}).setBlockType({abs(i % 16), k, abs(j % 16)}, BlockType::Dirt); //  Unter dem Noise-Wert gibt es nur Dirt
              } else if(k < (noise - 3) && k >= noise - 10) {
                m.getChunk({x, z}).setBlockType({abs(i % 16), k, abs(j % 16)}, BlockType::Stone);
              } else {
                m.getChunk({x, z}).setBlockType({abs(i % 16), k, abs(j % 16)}, BlockType::Air); //  Über dem Noise-Wert gibt es nur Air
              }
            }
          }
        }
      }
    }
  }
}


int ChunkGenerator::perlinNoise(int x, int z) {

  int x_min = x / m_octave;
  int x_max = x_min + 1;
  int z_min = z / m_octave;
  int z_max = z_min + 1;

  Vec2i a = {x_min, z_min};
  Vec2i b = {x_max, z_min};
  Vec2i c = {x_min, z_max};
  Vec2i d = {x_max, z_max};

  float random_a = randomPos(a);
  float random_b = randomPos(b);
  float random_c = randomPos(c);
  float random_d = randomPos(d);
  /*
    float random_a = randomPos(x_min, z_min);
    float random_b = randomPos(x_max, z_min);
    float random_c = randomPos(x_min, z_max);
    float random_d = randomPos(x_max, z_max);
  */
  // int interpolated = (int)((random_a + random_b + random_c + random_d) / 4);
  // interpolated = interpolated*64+64;
  float interpolated1 = interpolate(random_a, random_b, (x - x_min * m_octave) / m_octave);
  float interpolated2 = interpolate(random_c, random_d, (x - x_min * m_octave) / m_octave);
  float interpolated3 = interpolate(interpolated1, interpolated2, (z - z_min * m_octave) / m_octave);

  return interpolated3;
}

float ChunkGenerator::interpolate(float a, float b, float x) {
  float ft = (float)(x * 3.141);
  float f = (float)((1 - cos(ft)) * .5);
  float ret = a * (1 - f) + b * f;
  return ret;
}

float ChunkGenerator::randomPos(Vec2i vector) {
  // float ChunkGenerator::randomPos(int x,int z) {
  float var = 10000 * (sin(vector.x) + cos(vector.y) + tan(m_seed)); // randomzahl
  uniform_real_distribution<float> dist(74.f, 88.f);
  minstd_rand0 nrng(var);
  float wert = dist(nrng);
  return wert;
  /*int n = (int)x + (int)z*57;
  n=(n<<13);
  int nn = (n*(n*n*60493+19990303)+1376312589)&0xfffffff;
  cout <<1.0-((float)nn/1073741824.0) << endl;
  return 1.0-((float)nn/1073741824.0);*/
}
