#include "thread.h"
#include <windows.h>

int JIN_sleep(double time)
{
  Sleep(time);

  return 0;
}
