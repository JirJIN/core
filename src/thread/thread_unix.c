#include "thread.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct JIN_Thread {
  pthread_t pthread;
};

struct JIN_Thread * JIN_thread_create(JIN_THREAD_FN (*fn)(void *))
{
  struct JIN_Thread *thread;

  if (!(thread = malloc(sizeof(struct JIN_Thread)))) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  pthread_create(&thread->pthread, NULL, fn, NULL);

  return thread;
}

int JIN_thread_destroy(struct JIN_Thread *thread)
{
  free(thread);

  return 0;
}

int JIN_thread_join(struct JIN_Thread *thread)
{
  pthread_join(thread->pthread, NULL);
  return 0;
}

int JIN_sleep(double time)
{
  usleep(time * 1000);

  return 0;
}
