#include "thread.h"
#include <windows.h>

int JIN_sleep(double time)
{
  Sleep((DWORD) time);

  return 0;
}
