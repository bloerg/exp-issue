#include <cmath>
#include <iostream>
// Random Number Generator
#include "philox.h"
#include "uniform.hpp"

// choose random number generator
typedef r123::Philox4x32_R<7> RNG;

int main() {
  
  unsigned int WORKER=1;
  unsigned int seed=1000;
  int iteration=1;
  RNG rng;
  RNG::key_type k = {{WORKER, 0xdecafbad}};
  RNG::ctr_type c = {{0, seed, iteration, 0xBADC0DED}};
  RNG::ctr_type r;
//  std::cout << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] <<"\n";
  float f;
  for (int i=0; i<100000; i++) {
      ++c[0];
      r = rng(c, k);
//      std::cout << "r: " << r << "\n";
      f = r123::u01fixedpt<float>(r.v[i%4]);
      std::cout << f << "," << expf(f) << "\n";
  }
  return 0;
}

