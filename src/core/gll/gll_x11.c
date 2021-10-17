#include "gll.h"
#include <GL/glx.h>
#include "../logger/logger.h"

int JIN_gll(void)
{
  #define GLFN(ret, name, ...) \
    name = (name##proc *) glXGetProcAddress((const GLubyte *) #name); \
    if (!name) ERR_EXIT(-1, "Failed to load " #name "\n");

  JIN_GL_PROCS
  #undef GLFN

  return 0;
}
