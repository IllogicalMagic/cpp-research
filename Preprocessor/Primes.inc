// Generates sequence of prime numbers not bigger than MAX_NUM.
// Define anything only once.
#if !defined NUM
#define NUM (X / 300)
#endif

#if (NUM) <= (MAX_NUM) // Outer loop
// Check current number.
#include "IsPrime.inc"

// Print it if it is prime.
#if PRIME == 1
NUM,
#else
// Lengthen critical path.
#line X
#endif
#undef PRIME

// Go to next number.
#define CUR_CTR ((((X - 3) / 100) + 1) * 100)
#if (X % 300) < 100
#include "RewindCounter.inc"
#endif
#if (X % 300) < 200
#include "RewindCounter.inc"
#endif
#include "RewindCounter.inc"
#undef CUR_CTR

// Recurse.
#include "Primes.inc"

#else
// Lengthen critical path.
#line X
#line X
#endif // Outer loop

#undef NUM
