#include "../core.h"
#include <emscripten/html5.h>

#define SYM_LIST \
  X(f1, 0) \
  X(f2, 1) \
  X(f3, 2) \
  X(f4, 3) \
  X(a,  4) \
  X(d,  5) \
  X(s,  6) \
  X(w,  7)

#include <stdio.h>
EM_BOOL key_callback(int event_type, const EmscriptenKeyboardEvent *key_event, void *user_data)
{
  int value = event_type == EMSCRIPTEN_EVENT_KEYDOWN ? 1 : 0; /* Figure out math and remove conditional */
  printf("Value is: %d\n Key event is: %d\n\n", value, event_type);
  printf("Key code: %lu", key_event->keyCode);
  /* keyCode is deprecated but I need a numeric value */
  switch (key_event->keyCode) {
    #define X(jink, sym) case sym: JIN_inputv.keys.jink = value; break;

    SYM_LIST
    #undef X
  }

  return EM_TRUE;
}

int JIN_input_loop(void)
{
  emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, NULL, EM_TRUE, key_callback);

  return 0;
}
