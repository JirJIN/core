#include "env.h"

#include <stdio.h>

int error_handler(Display *display, XErrorEvent *event)
{
  char buff[512];
  XGetErrorText(display, event->error_code, buff, 512);
  fprintf(stderr, "GLX Error: %s\n", buff);
  return 0;
}

int JIN_env_init(struct JIN_Env *env)
{
  XInitThreads();

  if (!(env->x_display = XOpenDisplay(NULL))) {
    fprintf(stderr, "Could not open a display (X11)");
    return -1;
  }
  XSetErrorHandler(error_handler);

  env->screen_id = XDefaultScreen(env->x_display);
  
  env->border_pixel      = XBlackPixel(env->x_display, env->screen_id);
  env->background_pixel  = XWhitePixel(env->x_display, env->screen_id);
  
  env->wm_delete_window = XInternAtom(env->x_display, "WM_DELETE_WINDOW", False);

  return 0;
}

int JIN_env_quit(struct JIN_Env *env)
{
  XCloseDisplay(env->x_display);

  return 0;
}
