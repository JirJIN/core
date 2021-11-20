#include "thread.h"
#include <windows.h>
#include <process.h>

struct JIN_Thread {
  HANDLE handle;
};

#include <stdio.h>
struct JIN_Thread * JIN_thread_create(JIN_THREAD_FN (*fn)(void *))
{
  struct JIN_Thread *thread;

  if (!(thread = malloc(sizeof(struct JIN_Thread)))) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  if (!(thread->handle = (HANDLE) _beginthreadex(NULL, 0, fn, NULL, 0, NULL))) {
    fprintf(stderr, "Could not create a thread\n");
    return NULL;
  }

  return thread;
}

int JIN_thread_destroy(struct JIN_Thread *thread)
{
  free(thread);

  return 0;
}

int JIN_thread_join(struct JIN_Thread *thread)
{
  WaitForSingleObject(thread->handle, INFINITE);

  return 0;
}

int JIN_sleep(double time)
{
  Sleep((DWORD) time);

  return 0;
}
