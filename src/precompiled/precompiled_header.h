#define _USE_MATH_DEFINES
#include <math.h>
#include <QtCore>

#include <deque>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <set>

using std::unique_ptr;
using std::map;
using std::vector;
using std::string;
using std::set;
using std::deque;
using std::pair;
#if (__cplusplus <= 201103L) && !(defined _MSC_VER)
  /*!
   * \brief Construct a new object and wrap it in a unique_ptr
   */
  template<typename T, typename... Args>
  unique_ptr<T> make_unique (Args&&... args)
  {
    return unique_ptr<T> (new T (std::forward<Args> (args)...));
  }
#else
using std::make_unique;
#endif // __cplusplus <= 201103L


#include "common/common_algorithms.h"
