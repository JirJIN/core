#ifndef JIN_EVENT_H
#define JIN_EVENT_H

/*
 * EVENT
 *
 * Events and inputs from the user
 * Event just handles receiving inputs,
 * "input" in the main repo is where inputs are stored
 */

enum JIN_Eventt {
  JIN_EVENT_NONE, /* No event */
  JIN_EVENT_QUIT, /* Quit event */
  JIN_EVENT_KEY,  /* Keyboard event */
  JIN_EVENT_MOUSE /* Mouse event */
};

/* Keyboard events */
/*
 * Keyboard is bit complicated, going to
 * try on Windows first
 */
enum JIN_EventKeyt {
  JIN_EVENT_KEY_UP,
  JIN_EVENT_KEY_DOWN
};

struct JIN_EventKey {
  enum JIN_EventKeyt type;
  int  key;
};

/* Mouse events */

struct JIN_EventMouse {
  int dummy; /* Placeholder so VS stops complaining*/
};

/* General Event */

union JIN_Eventd {
  struct JIN_EventKey key;
};

struct JIN_Event {
  enum  JIN_Eventt type;
  union JIN_Eventd data;
};

/*
 * JIN_event_poll
 *
 * @desc
 *   Checks if there are any events
 * @param event
 *   Where to store event data
 * @return
 *    0 if there are no events
 *   !0 if there is an event
 */
int JIN_event_poll(struct JIN_Event *event);

#endif
