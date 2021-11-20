#include "window.h"

#ifdef __EMSCRIPTEN__
  #include "window_em.c"
#elif __linux__
  #include "window_x11.c"
#elif _WIN32
  #include "window_win32.c"
#else
  #error Platform not supported (JIN Window)
#endif
