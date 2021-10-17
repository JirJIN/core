#include "thread.h"

#ifdef __unix__
  #include "thread_unix.c"
#elif _WIN32
  #include "thread_win32.c"
#else
  #error Platform not supported (JIN Thread)
#endif
