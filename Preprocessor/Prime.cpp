#include <iostream>

#define X __COUNTER__

// Initialize counter to value of 2.
#if X - X
#endif

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
