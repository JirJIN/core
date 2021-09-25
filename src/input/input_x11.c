#include "input.h"
#include "../env/env.h"
#include <X11/Xlib.h>
#include "core.h"

#include <stdio.h>

int JIN_input_loop(void)
{
  XEvent xevent;
  while (!JIN_inputv.quit) {
    /*
     * Need to check if there are events
     * otherwise XNextEvent will deadlock
     */
    int queue = XPending(JIN_env.x_display);

    while (queue--) {
      XNextEvent(JIN_env.x_display, &xevent);

      switch (xevent.type) {
        case ClientMessage:
          if (xevent.xclient.data.l[0] == JIN_env.wm_delete_window) {
            JIN_inputv.quit = 1;
          }
          break;
      }
    }

  }

  return 0;
}
