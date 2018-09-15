// This file checks whether number is prime.
// If number is prime, PRIME == 1, otherwise PRIME == 0.
// Define anything only once.
#if !defined(PRIME)
#define PRIME 1
#endif

// X % 100 is in range [2, 3] and X is used 4 times.
#define DIVISOR (((((X) % 300) - 3) / 4) + 2)

// First divisor is (((3 - 3) / 4) + 2) == 2
#if ((DIVISOR * 2 - 2) <= ((NUM))) && PRIME

// N % D == 0, number is composite.
#if (NUM) % (DIVISOR) == 0
#undef PRIME
#define PRIME 0
#else
// Otherwise, go to next divisor.
#include "IsPrime.inc"
#endif

#else

// Lengthen critical path.
#line X
#line X

#endif
