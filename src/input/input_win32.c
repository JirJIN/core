#include "input.h"
#include "core.h"
#include <windows.h>

int JIN_input_loop(void)
{
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    switch (msg.message) {
    }
  }
  /* Message was WM_QUIT */
  JIN_inputv.quit = 1;

  return 0;
}
