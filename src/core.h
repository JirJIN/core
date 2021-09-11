#ifndef JIN_CORE_H
#define JIN_CORE_H

/*
 * CORE
 *
 * Window and input handling
 *
 * Only gonna make for linux and windows
 */

/*
 * Core functions
 * 
 * init   | Initialize JIN
 * quit   | Quit/Cleanup JIN
 * input  | Receive input
 * update | Game logic
 * draw   | Draw to the window
 * active | Check if the game is active
 */
int JIN_init(void);
int JIN_quit(void);

int JIN_input (void);
int JIN_update(void);
int JIN_draw  (void);
int JIN_active(void);

int JIN_dialog(const char *msg);

#endif
