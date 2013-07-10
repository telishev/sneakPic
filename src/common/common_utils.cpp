#include "common/common_utils.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif


void sleep_ms (int ms)
{
#ifdef _WIN32
  Sleep (ms);
#else
  timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms - ts.tv_sec * 1000) * 1000 * 1000;
  nanosleep (&ts, 0);
#endif

}
