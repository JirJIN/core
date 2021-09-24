#ifndef JIN_INPUT_H
#define JIN_INPUT_H

/*
 * INPUT
 * 
 * All input information
 */

struct JIN_Input {
  int quit;
};

#define JIN_INPUT_INIT(input) \
  input.quit = 0;

#endif