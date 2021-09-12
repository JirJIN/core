#include "core.h"

/*
 * Useful links
 * https://github.com/gamedevtech/X11OpenGLWindow
 * https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
 */

#include "window/window.h"
#include "event/event.h"
#include "env/env.h"

struct JIN_Window *root; /* Root window */
int                active;
struct JIN_Env     env; /* Environment variables */

/* CORE FUNCTIONS */

/*
 * JIN_init
 *
 * @desc
 *   Initialize JIN
 * @return
 *    0 on success
 *   !0 on failure
 */
int JIN_init(void)
{
  JIN_env_init(&JIN_env);
  root = JIN_window_create();
  
  active = 1;

  return 0;
}

/*
 * JIN_quit
 *
 * @desc
 *   Quit JIN
 * @return
 *   0 on success
 */
int JIN_quit(void)
{
  JIN_window_destroy(root);
  JIN_env_quit(&JIN_env);

  return 0;
}

/*
 * JIN_input
 *
 * @desc
 *   Handle input
 * @return
 *    0 on success
 *   !0 on failure
 */
#include <stdio.h>
int JIN_input(void)
{
  struct JIN_Event event;
  JIN_event_poll(&event);

  switch (event.type) {
    case JIN_EVENT_QUIT:
      active = 0;
      break;
    case JIN_EVENT_KEY:
      break;
    case JIN_EVENT_MOUSE:
      break;
  }

  if (event.type == JIN_EVENT_KEY) {
    if (event.data.key.type == JIN_EVENT_KEY_DOWN) {
      printf("Key is pressed\n");
      /* Escape key */
      if (event.data.key.key == 0x09) {
        active = 0;
      }
    }
  }

  return 0;
}

/*
 * JIN_update
 * 
 * @desc
 *   Update stuff
 * @return
 *   0 on success
 */
int JIN_update(void)
{
  return 0;
}

/*
 * JIN_draw
 *
 * @desc
 *   Draw stuff
 * @return
 *   0 on success
 */
int JIN_draw(void)
{
  return 0;
}

/*
 * JIN_active
 *
 * @desc
 *   Check if the game is active
 * @return
 *   !0 if active
 *    0 if not acive
 */
int JIN_active(void)
{
  return active;
}

/*
 * JIN_dialog
 *
 * @desc
 *   Create a dialog (pop-up box)
 *   to display a message to the user
 * @param msg
 *   String to display
 * @return
 *    0 on success
 *   !0 on failure
 */
int JIN_dialog(const char *msg)
{
  return 0;
}
