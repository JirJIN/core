#ifndef JIN_INPUT_H
#define JIN_INPUT_H

/*
 * INPUT
 * 
 * All input information
 */

struct JIN_Keys {
  int f1;
  int f2;
  int f3;
  int f4;
  int a;
  int d;
  int s;
  int w;
};

struct JIN_Input {
  int quit;
  struct JIN_Keys keys;
};

/*
 * JIN_input_loop
 *
 * @desc
 *   Continue getting inputs until
 *   a 'quit' input
 * @return
 */
int JIN_input_loop(void);

#endif
