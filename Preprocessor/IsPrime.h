// This file checks whether number is prime.
// If number is prime, PRIME == 1, otherwise PRIME == 0.
// Define anything only once.
#if !defined(PRIME)
#define PRIME 1
#endif

# if !defined(DIVISOR_X1)
// X % 100 is in range [2, 3] and X is used 4 times.
#define DIVISOR_X1 (((((X) % 300) - 3) / 4) + 2)
#endif

// First divisor is (((3 - 3) / 4) + 2) == 2
#if ((DIVISOR_X1 * 2 - 2) <= ((NUM_X1))) && PRIME

// N % D == 0, number is composite.
#if (NUM_X1) % (DIVISOR_X1) == 0
#undef PRIME
#define PRIME 0
#else
// Otherwise, go to next divisor.
#include "IsPrime.h"
#endif

#else

// Lengthen critical path.
#if X - X
#endif

#endif

#undef DIVISOR_X1
