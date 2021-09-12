#include "window.h"
#include <Windows.h>
#include "../env/env.h"

#include <stdio.h>

struct JIN_Window {
  HWND handle;
};

struct JIN_Window * JIN_window_create(void)
{
  struct JIN_Window *window;

  if (!(window = malloc(sizeof(struct JIN_Window)))) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  window->handle = CreateWindowEx(0, "TestThing", "Hay", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, JIN_env.main_instance, NULL);

  if (!window->handle) {
    fprintf(stderr, "Failed to create the window. Error code %d\n", GetLastError());
    return 0;
  }

  ShowWindow(window->handle, SW_NORMAL);

  return window;
}

int JIN_window_destroy(struct JIN_Window* window)
{
  free(window);

  return 0;
}