#include "env.h"

#ifdef __linux__
  #include "env_x11.c"
#elif _WIN32
#else
  #error Platform not supported (JIN Env)
#endif

struct JIN_Env JIN_env;
