#include <X11/Xlib.h>

struct JIN_Env {
  Display *x_display;
  Atom     wm_delete_window;
};
