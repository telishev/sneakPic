#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <functional>

#define DO_CONCATENATE(A,B) A ## B
#define CONCATENATE(A,B) DO_CONCATENATE(A,B)

class scope_guard
{
  std::function<void ()> m_exit_func;
public:
  scope_guard (std::function<void ()> exit_func) { m_exit_func = exit_func; }
  ~scope_guard () { if (m_exit_func) m_exit_func (); }

  void release () { m_exit_func = nullptr; }
};

#define DO_ON_EXIT(FUNC) \
  scope_guard CONCATENATE(__do_on_exit_,__LINE__) ([&] () { FUNC; })

#define CHECK_RET(RESULT,RET) if (!RESULT) return RET;
#define CHECK(RESULT) CHECK_RET (RESULT, false)

/// \file common/common_defines.h

#define EXPAND(X) X

#define FIX_UNUSED_SINGLE(x) (void) (x);

// Make a FOREACH macro
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) EXPAND (WHAT(X)FE_1(WHAT, __VA_ARGS__))
#define FE_3(WHAT, X, ...) EXPAND (WHAT(X)FE_2(WHAT, __VA_ARGS__))
#define FE_4(WHAT, X, ...) EXPAND (WHAT(X)FE_3(WHAT, __VA_ARGS__))
#define FE_5(WHAT, X, ...) EXPAND (WHAT(X)FE_4(WHAT, __VA_ARGS__))
#define FE_6(WHAT, X, ...) EXPAND (WHAT(X)FE_5(WHAT, __VA_ARGS__))
#define FE_7(WHAT, X, ...) EXPAND (WHAT(X)FE_6(WHAT, __VA_ARGS__))
#define FE_8(WHAT, X, ...) EXPAND (WHAT(X)FE_7(WHAT, __VA_ARGS__))
#define FE_9(WHAT, X, ...) EXPAND (WHAT(X)FE_8(WHAT, __VA_ARGS__))
#define FE_10(WHAT, X, ...) EXPAND (WHAT(X)FE_9(WHAT, __VA_ARGS__))
#define FE_11(WHAT, X, ...) EXPAND (WHAT(X)FE_10(WHAT, __VA_ARGS__))
#define FE_12(WHAT, X, ...) EXPAND (WHAT(X)FE_11(WHAT, __VA_ARGS__))
#define FE_13(WHAT, X, ...) EXPAND (WHAT(X)FE_12(WHAT, __VA_ARGS__))
#define FE_14(WHAT, X, ...) EXPAND (WHAT(X)FE_13(WHAT, __VA_ARGS__))
#define FE_15(WHAT, X, ...) EXPAND (WHAT(X)FE_14(WHAT, __VA_ARGS__))
#define FE_16(WHAT, X, ...) EXPAND (WHAT(X)FE_15(WHAT, __VA_ARGS__))
#define FE_17(WHAT, X, ...) EXPAND (WHAT(X)FE_16(WHAT, __VA_ARGS__))
#define FE_18(WHAT, X, ...) EXPAND (WHAT(X)FE_17(WHAT, __VA_ARGS__))
#define FE_19(WHAT, X, ...) EXPAND (WHAT(X)FE_18(WHAT, __VA_ARGS__))
#define FE_20(WHAT, X, ...) EXPAND (WHAT(X)FE_19(WHAT, __VA_ARGS__))
//... repeat as needed

#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,NAME,...) NAME
#define FOR_EACH(ACTION,...) \
  EXPAND (GET_MACRO(__VA_ARGS__,FE_20,FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1)(ACTION,__VA_ARGS__))

#define FIX_UNUSED(...) \
  EXPAND (do { FOR_EACH(FIX_UNUSED_SINGLE,__VA_ARGS__); } while (0))

void sleep_ms (int ms);


#endif // COMMON_UTILS_H
