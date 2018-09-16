// Prime numbers generation using only C
// preprocessor with __COUNTER__ extension.

#include <stdio.h>

#define X __COUNTER__

// Initialize counter to value of 2.
#if X - X
#endif

#define MAX_NUM 100
int a[] = {
  2,
#include "Primes.h"
};

#define SIZEOF_A(A) (sizeof(A)/sizeof(A[0]))

int main() {
  for (int i = i; i < SIZEOF_A(a) - 1; ++i) {
    printf("%d, ", a[i]);
  }
  printf("%d\n", a[SIZEOF_A(a) - 1]);
  return 0;
}
