#include "core/core.h"
#include "core/thread/thread.h"

int main(int argc, char *args[])
{
  JIN_init();

  struct JIN_Thread *game_thread_handle;
  if (!(game_thread_handle = JIN_thread_create(JIN_game_thread))) {
    ERR_EXIT(-1, "Could not create the game thread");
  }

  JIN_input_loop();

  JIN_thread_join(game_thread_handle);

  JIN_thread_destroy(game_thread_handle);
  JIN_quit();

  return 0;
}
