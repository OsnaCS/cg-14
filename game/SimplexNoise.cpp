#include "SimplexNoise.hpp"

#include <math.h>
#include <vector>
using namespace std;

#include <iostream>



double SimplexNoise::noise(double m_xin, double m_yin, int m_seed) {
  // Skewing and unskewing factors for dimensions
  const double m_F2 = 0.5 * (sqrt(3.0) - 1.0);
  const double m_G2 = (3.0 - sqrt(3.0)) / 6.0;

  vector<Grad> m_grad3{Grad(1,1,0),Grad(-1,1,0),Grad(1,-1,0),Grad(-1,-1,0),
               Grad(1,0,1),Grad(-1,0,1),Grad(1,0,-1),Grad(-1,0,-1),
                         Grad(0,1,1),Grad(0,-1,1),Grad(0,1,-1),Grad(0,-1,-1)};

  vector<short> m_p{151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};
    // To remove the need for index wrapping, double the permutation table length
    vector<short> m_perm(512);
    vector<short> m_permMod12(512);
    for(int i = 0; i < 512; i++) {
      m_perm[i] = m_p[(i + m_seed) & 255];
      m_permMod12[i] = (short)(m_perm[i] % 12);
    }


  // Noise contributions from the three corners
  double m_n0, m_n1, m_n2;

  // Skew the input space to determine which simplex we're in
  double m_s = (m_xin + m_yin) * m_F2;  // Hairy factor for 2D
  int m_i = SimplexNoise::fastfloor(m_xin + m_s);
  int m_j = SimplexNoise::fastfloor(m_yin + m_s);
  double m_t = (m_i + m_j) * m_G2;
  double m_x00 = m_i - m_t;   // Unskew the cell origin back to (x,y) space
  double m_y00 = m_j - m_t;
  double m_x0 = m_xin - m_x00;  // The x,y distances from the cell origin
  double m_y0 = m_yin - m_y00;
  // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
  int m_i1, m_j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if(m_x0 > m_y0) { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    m_i1 = 1; 
    m_j1 = 0;
  }
  else {        // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    m_i1 = 0;
    m_j1 = 1;
  }
  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    double m_x1 = m_x0 - m_i1 + m_G2; // Offsets for middle corner in (x,y) unskewed coords
    double m_y1 = m_y0 - m_j1 + m_G2;
    double m_x2 = m_x0 - 1.0 + 2.0 * m_G2; // Offsets for last corner in (x,y) unskewed coords
    double m_y2 = m_y0 - 1.0 + 2.0 * m_G2;
    // Work out the hashed gradient indices of the three simplex corners
    int m_ii = m_i & 255;
    int m_jj = m_j & 255;
    int m_gi0 = m_permMod12[m_ii + m_perm[m_jj]];
    int m_gi1 = m_permMod12[m_ii + m_i1 + m_perm[m_jj + m_j1]];
    int m_gi2 = m_permMod12[m_ii + 1 + m_perm[m_jj + 1]];
    // Calculate the contribution from the three corners
    double m_t0 = 0.5 - m_x0 * m_x0 - m_y0 * m_y0;
    if(m_t0 < 0) {
      m_n0 = 0.0;
    } 
    else {
      m_t0 *= m_t0;
      m_n0 = m_t0 * m_t0 * SimplexNoise::dot(m_grad3[m_gi0], m_x0, m_y0); // (x,y) of grad3 used for 2D gradient
    }
    double m_t1 = 0.5 - m_x1 * m_x1 - m_y1 * m_y1;
    if(m_t1 < 0) {
      m_n1 = 0.0;
    } else {
      m_t1 *= m_t1;
      m_n1 = m_t1 * m_t1 * SimplexNoise::dot(m_grad3[m_gi1], m_x1, m_y1);
    }
    double m_t2 = 0.5 - m_x2 * m_x2 - m_y2 * m_y2;
    if(m_t2 < 0) {
      m_n2 = 0.0;
    } else {
      m_t2 *= m_t2;
      m_n2 = m_t2 * m_t2 * SimplexNoise::dot(m_grad3[m_gi2], m_x2, m_y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (m_n0 + m_n1 + m_n2);
}


int SimplexNoise::noiseInt(int m_c, int m_d, double m_simpNoise) {
  int m_a = -1;
  int m_b = 1;
  m_simpNoise = ((m_d * m_simpNoise - m_c * m_simpNoise + m_b * m_c - m_d * m_a) / (m_b - m_a));

  // Caste m_simpNoise als Integer
  return(int)(m_simpNoise + 0.5);
}