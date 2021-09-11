#include "thread.h"
#include <unistd.h>

int JIN_sleep(double time)
{
  usleep(time * 1000);

  return 0;
}
