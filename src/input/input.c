#include "input.h"

#ifdef __linux__
  #include "input_x11.c"
#elif _WIN32
  #include "input_win32.c"
#else
  #error Platform not supported (JIN Input)
#endif
