#include <X11/Xlib.h>
#include <stdlib.h>
#include "../env/env.h"
#include <string.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>

struct JIN_Window {
  Window     window;
  Screen    *screen;
  GLXContext context;
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
 * TODO
 *
 * Somehow move the OpenGL stuff to a different
 * function
 */
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

  /* Check GLX Version */
  GLint glx_major, glx_minor;
  glXQueryVersion(JIN_env.x_display, &glx_major, &glx_minor);
  if (glx_major <= 1 && glx_minor < 3) {
    fprintf(stderr, "GLX 1.3 or greater is required\n");
    return NULL;
  }

  GLint glx_attribs[] = {
    GLX_X_RENDERABLE,  True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
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

  int fbcount;
  GLXFBConfig *fbc = glXChooseFBConfig(JIN_env.x_display, screen_id, glx_attribs, &fbcount);
  if (!fbc) {
    fprintf(stderr, "Could not get framebuffer\n");
    return NULL;
  }

  /* Pick the fb config/visual with the most samples/pixel */
  int fbc_best = -1, fbc_worst = -1;
  int samp_best = -1, samp_worst = 999;
  for (int i = 0; i < fbcount; ++i) {
    XVisualInfo *vi = glXGetVisualFromFBConfig(JIN_env.x_display, fbc[i]);
    if (vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(JIN_env.x_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib(JIN_env.x_display, fbc[i], GLX_SAMPLES,        &samples);

      if (fbc_best < 0 || (samp_buf && samples > samp_best)) {
        fbc_best = i;
        samp_best = samples;
      }
      if (fbc_worst < 0 || !samp_buf || samples < samp_worst) {
        fbc_worst = i;
      }
      samp_worst = samples;
    }
    XFree(vi);
  }

  GLXFBConfig best_fbc = fbc[fbc_best];
  XFree(fbc);

  XVisualInfo *visual = glXGetVisualFromFBConfig(JIN_env.x_display, best_fbc);

  if (!visual) {
    fprintf(stderr, "Could not create a visual window\n");
    return NULL;
  }

  if (screen_id != visual->screen) {
    fprintf(stderr, "Screen id does not match visual screen\n");
    return NULL;
  }

  /* Create the Window */
  window->window = XCreateSimpleWindow(JIN_env.x_display, XRootWindowOfScreen(window->screen), 0, 0, 480, 320, 1, XBlackPixel(JIN_env.x_display, screen_id), XWhitePixel(JIN_env.x_display, screen_id));

  XSelectInput(JIN_env.x_display, window->window, KeyPressMask | KeyReleaseMask);
  XSetWMProtocols(JIN_env.x_display, window->window, &JIN_env.wm_delete_window, 1);

  /* Create the GLX OpengGL Context */
  //glxCreate

  GLXContext (*glx_create_context_attribs_arb)(Display *, GLXFBConfig, GLXContext, Bool, const int *) = 0;
  glx_create_context_attribs_arb = (GLXContext (*)(Display *, GLXFBConfig, GLXContext, Bool, const int *)) glXGetProcAddressARB( (const GLubyte *) "glXCreateCibtextAttribsARB");

  const char *glx_extensions = glXQueryExtensionsString(JIN_env.x_display, screen_id);
  if (!glx_create_context_attribs_arb) {
    fprintf(stderr, "glXCreateContextAttribsARB() not found\n");
  }

  int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 2,
    GLX_CONTEXT_FLAGS_ARB,         GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    None
  };

  if (!is_extension_supported(glx_extensions, "GLX_ARB_createcontext")) {
    window->context = glXCreateNewContext(JIN_env.x_display, best_fbc, GLX_RGBA_TYPE, 0, True);
  }
  else {
    window->context = glx_create_context_attribs_arb(JIN_env.x_display, best_fbc, 0, True, context_attribs);
  }
  XSync(JIN_env.x_display, False);

  /* Make sure context is a direct context */
  if (!glXIsDirect(JIN_env.x_display, window->context)) {
    fprintf(stderr, "GLX context is not direct");
  }
  glXMakeCurrent(JIN_env.x_display, window->window, window->context);

  /* Raise the window */
  XClearWindow(JIN_env.x_display, window->window);
  XMapRaised(JIN_env.x_display, window->window);

  return window;
}

int JIN_window_destroy(struct JIN_Window *window)
{
  glXDestroyContext(JIN_env.x_display, window->context);

  XDestroyWindow(JIN_env.x_display, window->window);
 
  free(window);

  return 0;
}

int JIN_window_buffer_swap(struct JIN_Window *window)
{
  glXSwapBuffers(JIN_env.x_display, window->window);

  return 0;
}
