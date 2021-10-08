#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "../env/env.h"

/*
 * DIALOG
 *
 * Seperated this into its own file
 * for organization
 *
 * Functions:
 * setup   | Creates the window with all the attributes
 * loop    | Message loop for the window
 * message | Put text on the dialog box
 */
#define DIALOG_WIDTH  240
#define DIALOG_HEIGHT 160
#define STATE_ATOMS 4
/*
 * The documentation for these atoms is whack
 *
 * I believe I used the freedesktop specs for them
 */
static int dialog_setup(Window *msgbox, Window parent)
{
  XSetWindowAttributes attribs;
  Atom _NET_WM_WINDOW_TYPE, _NET_WM_WINDOW_TYPE_DIALOG, _NET_WM_NAME;
  
  attribs.border_pixel      = JIN_env.border_pixel;
  attribs.background_pixel  = JIN_env.background_pixel;
  attribs.override_redirect = True;
  attribs.event_mask        = ExposureMask;
  *msgbox = XCreateWindow(JIN_env.x_display, XRootWindow(JIN_env.x_display, JIN_env.screen_id),
      0, 0, DIALOG_WIDTH, DIALOG_HEIGHT, 0,
      CopyFromParent, InputOutput, CopyFromParent,
      CWBackPixel | CWBorderPixel | CWEventMask, &attribs);

  XSizeHints hints;
  hints.flags = PMinSize | PMaxSize | PResizeInc;
  hints.min_width  = hints.max_width  = DIALOG_WIDTH;
  hints.min_height = hints.max_height = DIALOG_HEIGHT;
  hints.width_inc  = hints.height_inc = 0;
  XSetWMNormalHints(JIN_env.x_display, *msgbox, &hints);
  
  Atom _NET_WM_STATE = XInternAtom(JIN_env.x_display, "_NET_WM_STATE", False);
  char *state_atoms_names[STATE_ATOMS] = {
    "_NET_WM_STATE_SKIP_TASKBAR",
    "_NET_WM_STATE_SKIP_PAGER",
    "_NET_WM_STATE_FOCUSED",
    "_NET_WM_STATE_MODAL"
  };
  Atom state_atoms[STATE_ATOMS];
  for (int i = 0; i < STATE_ATOMS; ++i) {
    state_atoms[i] = XInternAtom(JIN_env.x_display, state_atoms_names[i], False);
  }
  XChangeProperty(JIN_env.x_display, *msgbox, _NET_WM_STATE, XA_ATOM, 32, PropModeReplace, (unsigned char *) state_atoms, STATE_ATOMS);
  XSetTransientForHint(JIN_env.x_display, *msgbox, parent);

  _NET_WM_WINDOW_TYPE = XInternAtom(JIN_env.x_display, "_NET_WM_WINDOW_TYPE", False);
  _NET_WM_WINDOW_TYPE_DIALOG = XInternAtom(JIN_env.x_display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
  XChangeProperty(JIN_env.x_display, *msgbox, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace,
      (unsigned char *) &_NET_WM_WINDOW_TYPE_DIALOG, 1);
  XSetWMProtocols(JIN_env.x_display, *msgbox, &JIN_env.wm_delete_window,1 );

  return 0;
}

#include <string.h>
/*
 * TODO
 *
 * Figure out how to break up text, maybe even resize the
 * text box a little bit
 */
static int dialog_message(Window dialog, const char *message)
{
  int x, y, direction, ascent, descent;
  XCharStruct overall;
  int message_len = strlen(message);

  XTextExtents(JIN_env.font, message, message_len, &direction, &ascent, &descent, &overall);
  x = (DIALOG_WIDTH - overall.width) / 2;
  y = DIALOG_HEIGHT / 2 + (ascent - descent) / 2;

  XSetBackground(JIN_env.x_display, JIN_env.gc, JIN_env.background_pixel);
  XSetForeground(JIN_env.x_display, JIN_env.gc, JIN_env.border_pixel);
  XClearWindow(JIN_env.x_display, dialog);
  XDrawString(JIN_env.x_display, dialog, JIN_env.gc, x, y, message, message_len);

  return 0;
}

static int dialog_loop(Window dialog, const char *message)
{
  XEvent event;
  while (1) {
    XNextEvent(JIN_env.x_display, &event);
    if (event.xany.window == dialog) {
      if (event.type == ClientMessage && event.xclient.data.l[0] == JIN_env.wm_delete_window) {
        break;
      }
      else if (event.type == Expose) {
        dialog_message(dialog, message);
      }
    }
  }
  
  return 0;
}

