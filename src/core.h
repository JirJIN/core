#ifndef JIN_CORE_H
#define JIN_CORE_H

#include "input/input.h"

/*
 * CORE
 *
 * Window and input handling
 *
 * Only gonna make for linux and windows
 */

extern struct JIN_Input JIN_inputv; /* Volatile Input */
extern struct JIN_Input JIN_input; /* "Frame" Input */

/*
 * Core functions
 * 
 * init   | Initialize JIN
 * quit   | Quit/Cleanup JIN
 * input  | Receive input
 * update | Game logic
 * draw   | Draw to the window
 */
int JIN_init(void);
int JIN_quit(void);

int JIN_tick  (void);
int JIN_update(void);
int JIN_draw  (void);

int JIN_dialog(const char *msg);

#endif
