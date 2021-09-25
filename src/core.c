#include "core.h"
#include "glew/glew.h"
#include "time.h"
#include "thread/thread.h"

/*
 * Useful links
 * https://github.com/gamedevtech/X11OpenGLWindow
 * https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
 */

#include "window/window.h"
#include "event/event.h"
#include "env/env.h"

struct JIN_Window *root; /* Root window */
struct JIN_Env     env; /* Environment variables */

struct JIN_Input JIN_inputv;
struct JIN_Input JIN_input;

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

  JIN_INPUT_INIT(JIN_inputv);
  JIN_INPUT_INIT(JIN_input);

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

#define FPS         30
#define FRAME_DELAY (1000 / FPS)
int JIN_tick(void)
{
  clock_t frame_start, frame_end;
  double  frame_time;

  frame_start = clock();

  JIN_input = JIN_inputv;
  JIN_update();
  JIN_draw();

  frame_end = clock();
  frame_time = (frame_end - frame_start) / CLOCKS_PER_SEC / 1000;

  if (FRAME_DELAY > frame_time) {
    JIN_sleep(FRAME_DELAY - frame_time);
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
  glClearColor(0.0f, 0.2f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  JIN_window_buffer_swap(root);

  return 0;
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
