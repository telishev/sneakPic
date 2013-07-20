#ifndef MATH_DEFS_H
#define MATH_DEFS_H

#include <math.h>

#define MINIMAL_FOR_COMPARISON 1e-17

static inline bool are_equal (double first, double second)
{
  return fabs (first - second) < MINIMAL_FOR_COMPARISON;
}

#endif // MATH_DEFS_H
