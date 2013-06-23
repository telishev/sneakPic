/// \file  common/debug_utils.h
/// \brief Routines that can be useful for debugging a program.


#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H


#if defined _MSC_VER && (_MSC_VER < 1500 || _MSC_VER >= 1600) // Fix for msvc 2008
#  include <intrin.h>
#endif // defined _MSC_VER && (_MSC_VER < 1500 || _MSC_VER >= 1600)


/// In debug mode, pause the program.
/// In release mode, do nothing.
#ifdef _MSC_VER
#  if defined _DEBUG && (_MSC_VER < 1500 || _MSC_VER >= 1600)
#    define DEBUG_PAUSE(reason)                 __debugbreak ()
#  else // !(defined _DEBUG && (_MSC_VER < 1500 || _MSC_VER >= 1600))
#    define DEBUG_PAUSE(reason)                 do { } while (0)
#  endif // !(defined _DEBUG && (_MSC_VER < 1500 || _MSC_VER >= 1600))
#else // !_MSC_VER
#  ifdef TNAV_DEBUG
#    if defined (__i386__) || defined (__x86_64__)
#      define DEBUG_PAUSE(reason)               asm ("int $3")
#    else //! (defined (__i386__) || defined (__x86_64__))
#      define DEBUG_PAUSE(reason)               do { } while (0)
#    endif //! (defined (__i386__) || defined (__x86_64__))
#  else // !TNAV_DEBUG
#    define DEBUG_PAUSE(reason)                 do { } while (0)
#  endif // !TNAV_DEBUG
#endif // !_MSC_VER


/// In debug mode, pause the program if the expression is evaluated to false.
/// In release mode, simply evaluate the expression and ignore result.
/// \note The expression is still evaluated in release mode to avoid sneaky errors
///       caused by assert's with side effects.
#define DEBUG_ASSERT(expr)                      \
do                                              \
  {                                             \
    if (!(expr))                                \
      DEBUG_PAUSE ("Assertion failed");         \
  }                                             \
while (0)                                       \


/// In debug mode, pause the program and return return_val if the expression evaluated is to false.
/// In release mode, return return_val if the expression is evaluated to false.
#define ASSERT_RETURN(expr, ret_val)            \
do                                              \
  {                                             \
    if (!(expr))                                \
      {                                         \
        DEBUG_PAUSE ("Assertion failed");       \
        return ret_val;                         \
      }                                         \
  }                                             \
while (0)                                       \


#endif // DEBUG_UTILS_H
