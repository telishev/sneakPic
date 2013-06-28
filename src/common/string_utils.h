#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

static const int issseparator_array[256] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1 /*\t*/, 1 /*\n*/, 1 /*\v*/, 1 /*\f*/, 1 /*\r*/, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1 /*' '*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 /*','*/, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static inline int is_separator (const unsigned char c)
{
  return issseparator_array[c];
}

static inline void trim_separators_left (const char *&str)
{
  while (is_separator (*str))
    str++;
}

static const int isspace_array[256] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1 /*\t*/, 1 /*\n*/, 1 /*\v*/, 1 /*\f*/, 1 /*\r*/, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1 /*' '*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 /*','*/, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static inline int is_space (const unsigned char c)
{
  return isspace_array[c];
}

static inline void trim_whitespace_left (const char *&str)
{
  while (is_space (*str))
    str++;
}

static const int isalpha_array[256] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //< Uppercase letters
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //< Lowercase letters
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/// Analog of isalpha function from ctype.h (for default locale)
static inline int is_alpha (const unsigned char c)
{
  return isalpha_array[c];
}

static const int isalnum_array[256] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, //< Digits
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //< Uppercase letters
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //< Lowercase letters
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/// Analog of isalnum function from ctype.h (for default locale)
static inline int is_alnum (const unsigned char c)
{
  return isalnum_array [c];
}

/// returns next identifier (string consisting of numbers and letters)
/// moves str to the end of the identifier
static std::string get_next_identifier (const char *&str)
{
  const char *begin = str;
  trim_whitespace_left (str);
  while (is_alnum (*str))
    str++;

  return std::string (begin, str);
}

/**
  \brief convert string to double value, e.g 1.32E+3, -32D-4
  \return strtod return value
 */
/*
 *----------------------------------------------------------------------
 *
 * str_to_double --
 *
 *      This procedure converts a floating-point number from an ASCII
 *      decimal representation to internal double-precision format.
 *
 * Results:
 *      The return value is the double-precision floating-point
 *      representation of the characters in string.  If endPtr isn't
 *      0, then *endPtr is filled in with the address of the
 *      next character after the last one that was part of the
 *      floating-point number.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 * Modified source code for the "strtod" library procedure.
 *
 * Modifications include:
 *      1. Support for 'errno' setting removed.
 *      2. Support for locales removed.
 *      4. Support for out of range exponent values (to prevent FPE) added.
 *      6. Bug fix of the original code: support for values like
 *              0.0000000000000000012345 and 0000000000000000000012345
 *
 *----------------------------------------------------------------------
 *
 * Copyright (c) 1988-1993 The Regents of the University of California.
 * Copyright (c) 1994 Sun Microsystems, Inc.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *----------------------------------------------------------------------
 */
static inline
double str_to_double (
  const char *string,   /* A decimal ASCII floating-point number,
                         * optionally preceded by white space.
                         * Must have form "-I.FE-X", where I is the
                         * integer part of the mantissa, F is the
                         * fractional part of the mantissa, and X
                         * is the exponent.  Either of the signs
                         * may be "+", "-", or omitted.  Either I
                         * or F may be omitted, or both.  The decimal
                         * point isn't necessary unless F is present.
                          *  E and X may both be omitted (but not just one).
                         */
  const char **endPtr   /* If non-0, store terminating character's
                         * address here. */
  )
{
  /* Largest possible base 10 exponent.  Any exponent larger than this will already
     produce underflow or overflow, so there's no need to worry about additional digits. */
  static const int maxExponent = 511;
  /* Table giving binary powers of 10.  Entry is 10^2^i.  Used to convert decimal
     exponents into floating-point numbers. */
  static const double powersOf10[] = {
    10.,
    100.,
    1.0e4,
    1.0e8,
    1.0e16,
    1.0e32,
    1.0e64,
    1.0e128,
    1.0e256
  };

  int sign, expSign = 0, power10, exp_val;
  double fraction, dblExp;
  const double *d;
  register const char *p;
  register int c;
  int exp = 0;                  /* Exponent read from "EX" field. */
  int fracExp = 0;              /* Exponent that derives from the fractional
                                 * part.  Under normal circumstatnces, it is
                                 * the negative of the number of digits in F.
                                 * However, if I is very long, the last digits
                                 * of I get dropped (otherwise a long I with a
                                 * large negative exponent could cause an
                                 * unnecessary overflow on I alone).  In this
                                 * case, fracExp is incremented one for each
                                 * dropped digit. */
  int mantSize;                 /* Number of digits in mantissa. */
  int decPt;                    /* Number of mantissa digits BEFORE decimal
                                 * point. */
  const char *pExp;             /* Temporarily holds location of exponent
                                 * in string. */
  int is_dec_point;             /* True if decimal point presents. */

  /*
   * Strip off leading blanks and check for a sign.
   */

  p = string;
  while (is_separator (*p))
    {
      p += 1;
    }
  if (*p == '-')
    {
      sign = 1;
      p += 1;
    }
  else
    {
      if (*p == '+')
        {
          p += 1;
        }
      sign = 0;
    }

  /* Trim 0 left */
  if (*p == '0')
    {
      mantSize = 1;
      for ( ; *(++p) == '0'; ) ;
    }
  else
    {
      mantSize = 0;
    }

  /*
   * Count the number of digits in the mantissa (including the decimal
   * point), and also locate the decimal point.
   */

  if (*p != '.')
    {
      decPt = -1;
      for (;; mantSize += 1)
        {
          c = *p;
          if (!(c >= '0' && c <= '9'))
            {
              if ((c != '.') || (decPt >= 0))
                {
                  break;
                }
              decPt = mantSize;
            }
          p += 1;
        }

      /*
       * Now suck up the digits in the mantissa.  Use two integers to
       * collect 9 digits each (this is faster than using floating-point).
       * If the mantissa has more than 18 digits, ignore the extras, since
       * they can't affect the value anyway.
       */

      pExp = p;
      p -= mantSize;
      if (decPt < 0)
        {
          decPt = mantSize;
        }
      else
        {
          mantSize -= 1;            /* One of the digits was the point. */
        }
      if (mantSize > 18)
        {
          fracExp = decPt - 18;
          mantSize = 18;
        }
      else
        {
          fracExp = decPt - mantSize;
        }

      is_dec_point = (p[decPt] == '.');
    }
  else
    {
      is_dec_point = 1;

      /* Skip '.' and count '0' after '.' */
      for ( ; *(++p) == '0'; ) fracExp--;

      /*
      * Count the number of digits in the mantissa, the decimal point located already.
      */
      for (mantSize = 0; ; mantSize += 1)
        {
          c = *p;
          if (!(c >= '0' && c <= '9'))
            break;

          p += 1;
        }

      pExp = p;
      if (mantSize == 0) // all 0
        {
          fraction = 0.0;
          // read Exp and finish
          goto read_exp;
        }

      p -= mantSize;

      // we skip all left 0 and save fracExp
      // now we set maximum of mantissa to 18 digits
      if (mantSize > 18)
        mantSize = 18;

      fracExp -= mantSize;
    }

  if (mantSize == 0)
    {
      fraction = 0.0;
      p = string;
      goto done;
    }
  else
    {
      int frac1, frac2;
      frac1 = 0;
      for (; mantSize > 9; mantSize -= 1)
        {
          c = *p;
          p += 1;
          if (c == '.')
            {
              c = *p;
              p += 1;
            }
          frac1 = 10 * frac1 + (c - '0');
        }
      frac2 = 0;
      for (; mantSize > 0; mantSize -= 1)
        {
          c = *p;
          p += 1;
          if (c == '.')
            {
              c = *p;
              p += 1;
            }
          frac2 = 10 * frac2 + (c - '0');
        }
      fraction = (1.0e9 * frac1) + frac2;
    }

  /*
   * Skim off the exponent.
   */

read_exp:

  p = pExp;
  if (   (*p == 'E') || (*p == 'e')
      || ((is_dec_point) && ((*p == '-') || (*p == '+'))))
    {
      if (!(is_dec_point) || ((*p != '-') && (*p != '+')))
        p += 1;

      if (*p == '-')
        {
          expSign = 1;
          p += 1;
        }
      else
        {
          if (*p == '+')
            {
              p += 1;
            }
          expSign = 0;
        }
      while (*p >= '0' && *p <= '9')
        {
          exp = exp * 10 + (*p - '0');
          p += 1;
        }
    }

  if (expSign)
    {
      exp = fracExp - exp;
    }
  else
    {
      exp = fracExp + exp;
    }

  /*
   * Generate a floating-point number that represents the exponent.
   * Do this by processing the exponent one bit at a time to combine
   * many powers of 2 of 10. Then combine the exponent with the
   * fraction.
   */

  if (exp < 0)
    {
      expSign = 1;
      exp = -exp;
    }
  else
    {
      expSign = 0;
    }
  if (exp > maxExponent)
    {
      exp = maxExponent;
    }
  dblExp = 1.0;
  exp_val = 1;
  power10 = 1;
  for (d = powersOf10; exp != 0; exp >>= 1, d += 1, power10 <<= 1)
    {
      if (exp & 01)
        {
          exp_val += power10;
          if (exp_val > 308)
            {
              dblExp = 1e307;
              break;
            }
          dblExp *= *d;
        }
    }
  if (expSign)
    {
      fraction /= dblExp;
    }
  else
    {
      fraction *= dblExp;
    }

done:
  if (endPtr)
    {
      *endPtr = p;
    }

  if (sign)
    {
      return -fraction;
    }

  return fraction;
}


/// parses double from a string
/// string pointer is moved to an end of the parsed double
/// returns if parsing was successful
static inline
bool str_to_double (const char *&string, double &data)
{
  const char *endPtr = nullptr;
  data = str_to_double (string, &endPtr);
  bool ok = (string != endPtr);
  string = endPtr;
  return ok;
}

#endif // STRING_UTILS_H
