#include "event.h"
#include <X11/Xlib.h>
#include "../env/env.h"

int JIN_event_poll(struct JIN_Event *event)
{
  int     queue;
  XEvent  xevent;

  queue = XPending(JIN_env.x_display);

  if (queue) {
    XNextEvent(JIN_env.x_display, &xevent);

    switch (xevent.type) {
      case KeyPress:
        event->type = JIN_EVENT_KEY;
        event->data.key.type = JIN_EVENT_KEY_DOWN;
        event->data.key.key = xevent.xkey.keycode;
        break;
      case KeyRelease:
        event->type = JIN_EVENT_KEY;
        event->data.key.type = JIN_EVENT_KEY_UP;
        event->data.key.key = xevent.xkey.keycode;
        break;
    }
  }

  return queue;
}
