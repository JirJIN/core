#include "event/event.h"
#include <Windows.h>

int JIN_event_poll(struct JIN_Event *event)
{
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      event->type = JIN_EVENT_QUIT;
    }
    
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 1;
}