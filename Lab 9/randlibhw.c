#include "randlib.h"
#include<immintrin.h>

/* Initialize the hardware rand64 implementation.  */
extern unsigned long long hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

