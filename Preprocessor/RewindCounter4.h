#if !defined C4
#define C4 ((CUR_CTR4 - BEG4)/ 4)
#endif

#if (X) < CUR_CTR4
#define CUR_CTR (BEG4 + C4)
#include "RewindCounter.h"
#undef CUR_CTR

#define CUR_CTR (BEG4 + C4 * 2)
#include "RewindCounter.h"
#undef CUR_CTR

#define CUR_CTR (BEG4 + C4 * 3)
#include "RewindCounter.h"
#undef CUR_CTR

#define CUR_CTR (CUR_CTR4)
#include "RewindCounter.h"
#undef CUR_CTR
#endif

#undef C4
