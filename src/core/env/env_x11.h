#include <X11/Xlib.h>

struct JIN_Env {
  /* X11 Window Stuff */
  Display      *x_display;
  int           screen_id;
  GC            gc; /* Default graphics context */
  XFontStruct  *font; /* Default font */

  unsigned long background_pixel;
  unsigned long border_pixel;

  Atom          wm_delete_window;
};
