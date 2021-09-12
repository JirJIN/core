#include "event.h"

#ifdef __linux__
  #include "event_x11.c"
#elif _WIN32
  #include "event_win32.c"
#else
  #error Platform not supported (JIN Window)
#endif
