#ifndef JIN_THREAD_H
#define JIN_THREAD_H

/*
 * THREAD
 *
 * Stuff for multithreading
 *
 * Really it's just for sleep because if I created
 * a 'utility' module it would only have sleep, so
 * I decided to make a thread module instead
 */

/*
 * Thread functions
 *
 * sleep | Sleeps the thread the function was called from
 */

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
int JIN_sleep(double time);

#endif
