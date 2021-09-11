#include <X11/Xlib.h>
#include <stdlib.h>
#include "../env/env.h"

#include <stdio.h>

struct JIN_Window {
  Window   window;
  Screen  *screen;
};

struct JIN_Window * JIN_window_create(void)
{
  int screen_id;
  struct JIN_Window *window;

  if (!(window = malloc(sizeof(struct JIN_Window)))) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  window->screen = XDefaultScreenOfDisplay(JIN_env.x_display);
  screen_id = XDefaultScreen(JIN_env.x_display);

  window->window = XCreateSimpleWindow(JIN_env.x_display, XRootWindowOfScreen(window->screen), 0, 0, 480, 320, 1, XBlackPixel(JIN_env.x_display, screen_id), XWhitePixel(JIN_env.x_display, screen_id));

  XSelectInput(JIN_env.x_display, window->window, KeyPressMask | KeyReleaseMask);

  XClearWindow(JIN_env.x_display, window->window);
  XMapRaised(JIN_env.x_display, window->window);

  return window;
}

int JIN_window_destroy(struct JIN_Window *window)
{
  XDestroyWindow(JIN_env.x_display, window->window);
  XFree(window->screen);
  XCloseDisplay(JIN_env.x_display);
 
  free(window);

  return 0;
}

