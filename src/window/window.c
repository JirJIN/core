#include "window.h"

#ifdef __linux__
  #include "window_x11.c"
#elif _WIN32
  #include "window_win32.c"
#else
  #error Platform not supported (JIN Window)
#endif
