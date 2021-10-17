#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

static FILE            *stream;
static enum JIN_Loggerm mode;
static unsigned int     flags;

/*
 * JIN_logger_init
 *
 * @desc
 * @param mode
 * @param f
 * @return
 */
int JIN_logger_init(enum JIN_Loggerm m, unsigned int f)
{
  switch (m) {
    case JIN_LOGGER_FILE:
      if (!(stream = fopen("log.txt", "w"))) return -1;
      setvbuf(stream, NULL, _IONBF, 0);
      break;
    default:
      stream = stderr;
      break;
  }
  flags = f;

  return 0;
}

/*
 * JIN_logger_quit
 *
 * @desc
 * @return
 */
int JIN_logger_quit(void)
{
  switch (mode) {
    case JIN_LOGGER_FILE:
      fclose(stream);
      break;
    default:
      break;
  }

  return 0;
}

/*
 * JIN_logger_log
 *
 * @desc
 * @param mode
 * @param msg
 * @return
 */
int JIN_logger_log(enum JIN_Loggerm m, const char *msg, ...)
{
  va_list args;
  va_start(args, msg);

  if (!(flags & m)) return -1;
  switch (m) {
    case JIN_LOGGER_LOG:
      fprintf(stream, "LOG: ");
      vfprintf(stream, msg, args);
      fprintf(stream, "\n");
      break;
    case JIN_LOGGER_DBG:
      fprintf(stream, "DEBUG: ");
      vfprintf(stream, msg, args);
      fprintf(stream, "\n");
      break;
    case JIN_LOGGER_ERR:
      fprintf(stream, "ERROR: ");
      vfprintf(stream, msg, args);
      fprintf(stream, "\n");
      break;
    default:
      break;
  }

  va_end(args);

  return 0;
}
