#ifndef MATH_DEFS_H
#define MATH_DEFS_H

#include <math.h>

#define MINIMAL_FOR_COMPARISON 1e-6

static inline bool are_equal (double first, double second)
{
  return 3 * fabs (first - second) < (first + second + 1.0) * MINIMAL_FOR_COMPARISON;
}

static inline double cubed (double x) { return x * x * x; }
static inline double squared (double x) { return x * x; }

#endif // MATH_DEFS_H
