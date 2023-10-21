#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "lib.h"
#define COLS    80  /* Number of columns in terminal */
#define ROWS    25  /* Number of rows in terminal */


typedef struct termin {
	unsigned char terminal_buffer[128];  //define the buffer of 128 bytes according to the requirement
	uint32_t buffer_iterator;
} termin_t;

extern uint32_t terminal_clear();
extern uint32_t terminal_init();
extern uint32_t terminal_read(unsigned char input);
extern void terminal_write();
extern uint32_t terminal_display(unsigned char input);
extern uint32_t terminal_delete();
extern uint32_t buffer_clear();

#endif 

