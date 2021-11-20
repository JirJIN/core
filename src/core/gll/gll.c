#include "gll.h"

#ifdef __EMSCRIPTEN__
  #include "gll_em.c"
#elif __linux__
  #include "gll_x11.c"
#elif _WIN32
  #include "gll_win32.c"
#else
  #error Platform not supported (JIN Thread)
#endif
