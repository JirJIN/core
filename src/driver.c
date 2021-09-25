#include "core.h"
#include "thread/thread.h"
#include "window/window.h"
#include <stdio.h>
#include "glew/glew.h"

extern struct JIN_Window *root;

JIN_THREAD_FN game_thread(void *data)
{
  JIN_window_make_current(root);
  GLenum err;
  if ((err = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return 0;
  }

  while (1) {
    if (JIN_input.quit) break;
    JIN_tick();
  }

  return 0;
}

int main(int argc, char *args[])
{
  JIN_init();

  struct JIN_Thread *game_thread_handle;
  if (!(game_thread_handle = JIN_thread_create(game_thread))) {
    return -1;
  }

  JIN_input_loop();

  JIN_thread_join(game_thread_handle);

  JIN_thread_destroy(game_thread_handle);
  JIN_quit();

  return 0;
}
