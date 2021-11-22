#ifndef JIN_ENV_H
#define JIN_ENV_H

/*
 * ENVIRONMENT
 *
 * Basically globals for core
 * An example would be the X11 display
 */

#ifdef __EMSCRIPTEN__
  #include "env_em.h"
#elif __linux__
  #include "env_x11.h"
#elif _WIN32
  #include "env_win32.h"
#else
  #error Platform not supported (JIN Env)
#endif

int JIN_env_init(struct JIN_Env *env);
int JIN_env_quit(struct JIN_Env *env);

extern struct JIN_Env JIN_env;

#endif
