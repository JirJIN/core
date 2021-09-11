#include "env.h"

#include <stdio.h>

int JIN_env_init(struct JIN_Env *env)
{
  if (!(env->x_display = XOpenDisplay(NULL))) {
    fprintf(stderr, "Could not open a display (X11)");
    return -1;
  }

  return 0;
}

int JIN_env_quit(struct JIN_Env *env)
{
  XCloseDisplay(env->x_display);

  return 0;
}
