#include <X11/Xlib.h>
#include <stdlib.h>
#include "../env/env.h"
#include <string.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>

struct JIN_Window {
  Window               window;
  Screen              *screen;
  GLXContext           context;
  XSetWindowAttributes attribs;
  XVisualInfo         *visual;
};

/*
 * is_extension_supported
 *
 * @desc
 *   Does some OpenGL stuff
 *   tbh idk, I'll read the docs later
 * @param extension_list
 * @param extension
 * @return
 *   !0 if supported
 *    0 if not supported
 */
static int is_extension_supported(const char *extension_list, const char *extension)
{
  const char *start;
  const char *where, *terminator;

  /* There are no spaces in extension names */
  where = strchr(extension, ' ');
  if (where || *extension == '\0') {
    return 0;
  }

  start = extension_list;
  while (1) {
    where = strstr(start, extension);

    if (!where) {
      break;
    }

    terminator = where + strlen(extension);

    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return 1;
      }
    }

    start = terminator;
  }

  return 0;
}

/*
 * JIN_window_gl_setup
 *
 * @desc
 * @param window
 * @param screen_id
 * @return
 */
static int JIN_window_gl_setup(struct JIN_Window *window, int screen_id)
{
  /* Check GLX Version */
  GLint glx_major, glx_minor;
  glXQueryVersion(JIN_env.x_display, &glx_major, &glx_minor);
  if (glx_major <= 1 && glx_minor < 3) {
    fprintf(stderr, "GLX 1.3 or greater is required\n");
    return -1;
  }

  GLint glx_attribs[] = {
    GLX_X_RENDERABLE,  True,
    GLX_DRAWABLE_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE,      8,
    GLX_GREEN_SIZE,    8,
    GLX_BLUE_SIZE,     8,
    GLX_ALPHA_SIZE,    8,
    GLX_DEPTH_SIZE,    24,
    GLX_STENCIL_SIZE,  8,
    GLX_DOUBLEBUFFER,  True,
    None
  };

  int fb_count;
  GLXFBConfig *fb_configs = glXChooseFBConfig(JIN_env.x_display, screen_id, glx_attribs, &fb_count);
  if (!fb_configs) {
    fprintf(stderr, "Could not get framebuffer config\n");
    return -1;
  }

  /* Pick the fb_config/visual with the most samples/pixel */
  int fb_config_best = -1, fb_config_worst = -1;
  int sample_best = -1, sample_worst = 999;
  for (int i = 0; i < fb_count; ++i) {
    XVisualInfo *vi;
    int sample_buffer, samples;
    
    if (!(vi = glXGetVisualFromFBConfig(JIN_env.x_display, fb_configs[i]))) {
      fprintf(stderr, "Could not get visual info\n");
      return -1;
    }

    glXGetFBConfigAttrib(JIN_env.x_display, fb_configs[i], GLX_SAMPLE_BUFFERS, &sample_buffer);
    glXGetFBConfigAttrib(JIN_env.x_display, fb_configs[i], GLX_SAMPLES,        &samples);

    if (fb_config_best < 0 || (sample_buffer && samples > sample_best)) {
      fb_config_best = i;
      sample_best = samples;
    }

    if (fb_config_worst < 0 || !sample_buffer || samples < sample_worst) {
      fb_config_worst = i;
    }

    sample_worst = samples;

    XFree(vi);
  }

  GLXFBConfig fb_config = fb_configs[fb_config_best];
  XFree(fb_configs);

  if (!(window->visual = glXGetVisualFromFBConfig(JIN_env.x_display, fb_config))) {
    fprintf(stderr, "Could not create a visual window\n");
    return -1;
  }

  if (screen_id != window->visual->screen) {
    fprintf(stderr, "Screen id does not match visual screen\n");
    return -1;
  }

  /* Actually create the context */
  GLXContext (*glx_create_context_attribs_arb)(Display *, GLXFBConfig, GLXContext, Bool, const int *) = NULL;
  glx_create_context_attribs_arb = (GLXContext (*)(Display *, GLXFBConfig, GLXContext, Bool, const int *)) glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");
  if (!glx_create_context_attribs_arb) {
    fprintf(stderr, "glXCreateContextAttribsARB() not found \n");
  }

  const char *glx_extensions = glXQueryExtensionsString(JIN_env.x_display, screen_id);
  
  int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 2,
    GLX_CONTEXT_FLAGS_ARB,         GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    None
  };

  if (!is_extension_supported(glx_extensions, "GLX_ARB_createcontext")) {
    window->context = glx_create_context_attribs_arb(JIN_env.x_display, fb_config, 0, True, context_attribs);
  }
  else {
    window->context = glx_create_context_attribs_arb(JIN_env.x_display, fb_config, 0, True, context_attribs);
  }
  XSync(JIN_env.x_display, False);

  /* Check if the context is direct */
  if (!glXIsDirect(JIN_env.x_display, window->context)) {
    fprintf(stderr, "GLX context is not direct");
  }

  return 0;
}

struct JIN_Window * JIN_window_create(void)
{
  int screen_id;
  struct JIN_Window *window;

  if (!(window = malloc(sizeof(struct JIN_Window)))) {
    fprintf(stderr, "Out of memory\n");
    return NULL;
  }

  window->screen = XDefaultScreenOfDisplay(JIN_env.x_display);
  screen_id = XDefaultScreen(JIN_env.x_display);

  if (JIN_window_gl_setup(window, screen_id)) {
    fprintf(stderr, "Could not set up OpenGL\n");
    return NULL;
  }

  /* Create the Window */
  window->attribs.border_pixel      = XBlackPixel(JIN_env.x_display, screen_id);
  window->attribs.background_pixel  = XWhitePixel(JIN_env.x_display, screen_id);
  window->attribs.override_redirect = True;
  window->attribs.colormap          = XCreateColormap(JIN_env.x_display, RootWindow(JIN_env.x_display, screen_id), window->visual->visual, AllocNone);
  window->attribs.event_mask        = ExposureMask;
  window->window = XCreateWindow(JIN_env.x_display, RootWindow(JIN_env.x_display, screen_id), 0, 0, 480, 320, 0, window->visual->depth, InputOutput, window->visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &window->attribs);

  XSelectInput(JIN_env.x_display, window->window, KeyPressMask | KeyReleaseMask);
  XSetWMProtocols(JIN_env.x_display, window->window, &JIN_env.wm_delete_window, 1);
  XSizeHints hints;
  hints.flags = PMinSize | PMaxSize | PResizeInc;
  hints.min_width  = hints.max_width  = 480;
  hints.min_height = hints.max_height = 320;
  hints.width_inc  = hints.height_inc = 0;
  XSetWMNormalHints(JIN_env.x_display, window->window, &hints);

  /* Raise the window */
  XClearWindow(JIN_env.x_display, window->window);
  XMapRaised(JIN_env.x_display, window->window);

  return window;
}

/* 
 * TODO
 *
 * Memory leaked:
 */
int JIN_window_destroy(struct JIN_Window *window)
{
  glXMakeCurrent(JIN_env.x_display, None, NULL);
  glXDestroyContext(JIN_env.x_display, window->context);

  XFree(window->visual);
  XFreeColormap(JIN_env.x_display, window->attribs.colormap);
  XDestroyWindow(JIN_env.x_display, window->window);
 
  free(window);

  return 0;
}

int JIN_window_buffer_swap(struct JIN_Window *window)
{
  glXSwapBuffers(JIN_env.x_display, window->window);

  return 0;
}

int JIN_window_make_current(struct JIN_Window *window)
{
  /* Set the GLX OpengGL Context */
  glXMakeCurrent(JIN_env.x_display, window->window, window->context);
  
  return 0;
}
