#include "core.h"
#include "thread/thread.h"
#include "window/window.h"
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "glew/glew.h"

extern struct JIN_Window *root;

/*
 * If somehow JIN_draw() can be called
 * from this thread after the WM_SIZE callback
 * is recieved, there will be no more problems
 * with Windows
 */
unsigned game_thread(void *data)
{
  //printf("Running game thread\n");
  JIN_window_make_current(root);
  GLenum err;
  if ((err = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return -1;
  }

  while (JIN_active()) {
    if (JIN_input.quit) break;
    JIN_tick();
  }

  return 0;
}

#include "glew/glew.h"
int main(int argc, char *args[])
{
  JIN_init();

  printf("Creating game thread\n");
  HANDLE game_thread_id;
  if (!(game_thread_id = (HANDLE) _beginthreadex(NULL, 0, game_thread, NULL, 0, NULL))) {
    fprintf(stderr, "Could not create a thread\n");
  }

  printf("Starting window loop\n");
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    switch (msg.message) {
    }
  }
  /* Message was WM_QUIT */
  JIN_inputv.quit = 1;

  WaitForSingleObject(game_thread_id, INFINITE);

  JIN_quit();

  //printf("Type something to quit\n");
  //getchar();

  return 0;
}
