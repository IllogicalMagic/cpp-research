// Prime numbers generation using only C
// preprocessor with __COUNTER__ extension.

#include <stdio.h>

#define X __COUNTER__

// Rewind counter to initial value (901).
// Internally, inner loop counter is equal counter modulo 300,
// outer loop counter is equal counter divided by 300.
// Start number will be 3.
#define BEG4 0
#define CUR_CTR4 400
#include "RewindCounter4.inc"
#undef CUR_CTR4
#undef BEG4

#define BEG4 400
#define CUR_CTR4 800
#include "RewindCounter4.inc"
#undef CUR_CTR4

#define CUR_CTR 901
#include "RewindCounter.inc"
#undef CUR_CTR

#define MAX_NUM 100
int a[] = {
  2,
#include "Primes.inc"
};

#define SIZEOF_A(A) (sizeof(A)/sizeof(A[0]))

int main() {
  for (int i = i; i < SIZEOF_A(a) - 1; ++i) {
    printf("%d, ", a[i]);
  }
  printf("%d\n", a[SIZEOF_A(a) - 1]);
  return 0;
}
