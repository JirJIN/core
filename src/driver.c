#include "core.h"
#include "thread/thread.h"

int main(int argc, char *args[])
{
  JIN_init();

  struct JIN_Thread *game_thread_handle;
  if (!(game_thread_handle = JIN_thread_create(JIN_game_thread))) {
    return -1;
  }

  JIN_input_loop();

  JIN_thread_join(game_thread_handle);

  JIN_thread_destroy(game_thread_handle);
  JIN_quit();

  return 0;
}
