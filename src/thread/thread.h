#ifndef JIN_THREAD_H
#define JIN_THREAD_H

/*
 * THREAD
 *
 * Stuff for multithreading
 *
 * All thread functions should return 0,
 * and the return type is JIN_THREAD_FN
 *
 * JIN_THREAD_FN yay_thread(void *)
 * {
 *   // Do stuff
 *   return 0;
 * }
 *
 * This is kinda rushed, and may not be
 * that clean/powerful
 *
 * The JIN_Thread struct is a handle to
 * a thread, it needs to be destroyed, but
 * note that it does not affect the thread
 * that it creates. (Destroy the struct
 * after doing a join)
 */

#ifdef __unix__
  #define JIN_THREAD_FN void *
#elif _WIN32
  #define JIN_THREAD_FN unsigned int
#else
  #error Platform not supported (JIN Thread)
#endif

struct JIN_Thread;

/*
 * Thread functions
 *
 * create  | Create a thread (it starts running when this is called)
 * destroy | Destroys a JIN_Thread struct (no affect on the thread)
 * join    | Wait for a thread to finish
 * sleep   | Sleeps the thread the function was called from
 */

/*
 * JIN_thread_create
 *
 * @desc
 * @param fn
 * @return
 */
struct JIN_Thread * JIN_thread_create (JIN_THREAD_FN (*fn)(void *));

/*
 * JIN_thread_destroy
 *
 * @desc
 *   Frees up a JIN_Thread struct, it does not do
 *   anything to the actual thread
 * @param thread
 * @return
 */
int                 JIN_thread_destroy(struct JIN_Thread *thread);

/*
 * JIN_thread_join
 *
 * @desc
 * @param thread
 * @return
 */
int                 JIN_thread_join   (struct JIN_Thread *thread);

/*
 * JIN_sleep
 *
 * @desc
 *   Sleep function so the engine
 *   doesn't use 100% of the cpu
 * @param time
 *   Time in ms
 * @return
 *   0 on success
 */
int                 JIN_sleep         (double time);

#endif
