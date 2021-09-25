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
