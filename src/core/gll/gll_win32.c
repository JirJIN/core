#include "gll.h"
#include "../logger/logger.h"

void * get_gl_proc_addr(const char* name)
{
  void * p = (void *) wglGetProcAddress(name);
  if (p == 0 || p == (void *) 0x1 || p == (void *) 0x2 || p == (void *) 0x3 || p == (void *) -1) {
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      ERR_EXIT(NULL, "Cannot open opengl32.dll");
    }
    p = (void *) GetProcAddress(module, name);
  }

  return p;
}

int JIN_gll(void)
{
  #define GLFN(ret, name, ...) \
    name = (name##proc *) get_gl_proc_addr(#name); \
    if (!name) ERR_EXIT(-1, "Failed to load " #name "\n");

  JIN_GL_PROCS
  #undef GLFN

  return 0;
}