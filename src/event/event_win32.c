#include "event/event.h"
#include <Windows.h>

#include <stdio.h>
int JIN_event_poll(struct JIN_Event *event)
{
  MSG msg;
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    switch (msg.message) {
      case WM_QUIT:
        event->type = JIN_EVENT_QUIT;
        break;
      default:
        event->type = JIN_EVENT_NONE;
        return 0;
    }
  }
  else {
    event->type = JIN_EVENT_NONE;
    return 0;
  }

  return 1;
}