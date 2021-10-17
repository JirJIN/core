#include "gll.h"
#include <GL/glx.h>

#include <stdio.h>
int JIN_gll(void)
{
  #define GLFN(ret, name, ...) \
    name = (name##proc *) glXGetProcAddress((const GLubyte *) #name); \
    if (!name) { printf("Failed to load " #name "\n"); return -1; }

  JIN_GL_PROCS
  #undef GLFN

  return 0;
}
