#include "core.h"

/*
 * Useful links
 * https://github.com/gamedevtech/X11OpenGLWindow
 * https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
 */

/* Currently linux only */

#ifdef __linux__
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #include <X11/keysymdef.h>
#elif _WIN32
#else
  #error Platform not supported (Core)
#endif

#include <stdio.h>

static Display *display;
static Window   window;
static Screen  *screen;

/*
 * core_init
 *
 * @desc
 * @return
 */
int JIN_core_init(void)
{
  int      screen_id;
  XEvent   event;

  if (!(display = XOpenDisplay(NULL))) {
    printf("Can't open the display\n");
  }
  screen = XDefaultScreenOfDisplay(display);
  screen_id = XDefaultScreen(display);

  window = XCreateSimpleWindow(display, XRootWindowOfScreen(screen), 0, 0, 480, 320, 1,
      XBlackPixel(display, screen_id), XWhitePixel(display, screen_id));

  XSelectInput(display, window, KeyPressMask | KeyReleaseMask | KeymapStateMask);

  XClearWindow(display, window);
  XMapRaised(display, window);

  char str[25];
  KeySym keysym = 0;
  int len = 0;
  int running = 1;

  while (running) {
    XNextEvent(display, &event);

    switch (event.type) {
      case KeymapNotify:
        XRefreshKeyboardMapping(&event.xmapping);
        break;
      case KeyPress:
        len = XLookupString(&event.xkey, str, 25, &keysym, NULL);
        if (len > 0) {
          printf("Key pressed: %s\n", str);
        }
        if (keysym == XK_Escape) {
          running = 0;
        }
        break;
      case KeyRelease:
        len = XLookupString(&event.xkey, str, 25, &keysym, NULL);
        if (len > 0) {
          printf("Key released: %s\n", str);
        }
        break;
    }
  }

  return 0;
}

/*
 * core_quit
 *
 * @desc
 * @return
 */
int JIN_core_quit(void)
{
  XDestroyWindow(display, window);
  XFree(screen);
  XCloseDisplay(display);

  return 0;
}
