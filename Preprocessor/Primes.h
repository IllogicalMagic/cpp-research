// Generates sequence of prime numbers not bigger than MAX_NUM.
// Define anything only once.
#if !defined NUM_X1
#define NUM_X1 ((X / 300) + 3)
#endif

#if (NUM_X1) <= (MAX_NUM) // Outer loop
// Check current number.
#include "IsPrime.h"

// Print it if it is prime.
#if PRIME == 1
NUM_X1,
#else
// Lengthen critical path.
#if X
#endif

#endif
#undef PRIME

// Go to next number.
#define CUR_CTR ((((X - 3) / 100) + 1) * 100)
#if (X % 300) < 100
#include "RewindCounter.h"
#endif
#if (X % 300) < 200
#include "RewindCounter.h"
#endif
#include "RewindCounter.h"
#undef CUR_CTR

// Recurse.
#include "Primes.h"

#else
// Lengthen critical path.
#if X - X
#endif

#endif // Outer loop

#undef NUM_X1
