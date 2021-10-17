#ifndef JIN_LOGGER
#define JIN_LOGGER

/*
 * LOGGER
 *
 * Log stuff to a file
 *
 * TYPES:
 * enum JIN_Loggerm | Logger modes
 */
enum JIN_Loggerm {
  /* Log modes */
  JIN_LOGGER_LOG = 0x1,
  JIN_LOGGER_DBG = 0x2,
  JIN_LOGGER_ERR = 0x4,
  /* Output modes */
  JIN_LOGGER_CONSOLE = 0,
  JIN_LOGGER_FILE
};

/*
 * Error Functions
 *
 * init | Initialize logger
 * quit | Quit logger
 * log  | Log a message
 */
int JIN_logger_init(enum JIN_Loggerm mode, unsigned int flags);
int JIN_logger_quit(void);
int JIN_logger_log (enum JIN_Loggerm mode, const char *msg, ...);

#define LOG(mode, msg, ...) JIN_logger_log(JIN_LOGGER_##mode, msg, ##__VA_ARGS__);
#define ERR_EXIT(ret, msg, ...) { JIN_logger_log(JIN_LOGGER_ERR, msg, ##__VA_ARGS__); return ret; }

#endif
