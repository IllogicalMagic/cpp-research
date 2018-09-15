#include <iostream>

#define X __COUNTER__

// Rewind counter to initial value (900).
// Internally, inner loop counter is equal counter modulo 300,
// outer loop counter is equal counter divided by 300.
// Start number will be 3.
#define BEG4 0
#define CUR_CTR4 400
#include "RewindCounter4.h"
#undef CUR_CTR4
#undef BEG4

#define BEG4 400
#define CUR_CTR4 800
#include "RewindCounter4.h"
#undef CUR_CTR4

#define CUR_CTR 901
#include "RewindCounter.h"
#undef CUR_CTR

#define MAX_NUM 100
int a[] = {
  2,
#include "Primes.h"
};

int main() {
  for (int p : a) {
    if (p)
      std::cout << p << ((*(std::end(a) - 1) == p) ? "" : ", ");
    else
      std::cout << std::endl;
  }
  std::cout << std::endl;
  return 0;
}
