// Random Number Generator
#include "philox.h"
#include "u01fixedpt.h"

//~ #pragma OPENCL EXTENSION cl_intel_printf : enable


//#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// 256 threads per block ensures the possibility of full occupancy
// for all compute capabilities if thread count small enough
//~ #define WORKERS_PER_BLOCK 256
//~ #define WORKER get_global_id(0)


__kernel void computeExp(  
  __global float* d_exps,
  __global float* d_rngs
  )
{

  unsigned int WORKER=1;
  unsigned int seed=1000;
  int iteration=1;
  float f;

  philox4x32_key_t k = {{WORKER, 0xdecafbad}};
  philox4x32_ctr_t c = {{0, seed, iteration, 0xBADC0DED}};//0xBADCAB1E
  philox4x32_ctr_t r;
  
  
  for (size_t i = 0; i < 100000; i++) {
      ++c.v[0];
      r = philox4x32_R(7, c, k);
      f = u01fixedpt_open_open_32_24(r.v[i%4]); // 24_32 = float;  64_53 = double
      d_exps[i] = exp(f);
      d_rngs[i] = f;
  }
}



