#include "core.h"
#include "time.h"
#include "thread/thread.h"

#define FPS         30
#define FRAME_DELAY (1000 / FPS)

int main(int argc, char *args[])
{
  JIN_init();

  while (JIN_active()) {
    clock_t frame_start, frame_end;
    double  frame_time;

    frame_start = clock();

    JIN_input();
    JIN_update();
    JIN_draw();

    frame_end = clock();
    frame_time = (frame_end - frame_start) / CLOCKS_PER_SEC / 1000;
     
    if (FRAME_DELAY > frame_time) {
      JIN_sleep(FRAME_DELAY - frame_time);
    }
  }

  JIN_quit();

  return 0;
}
