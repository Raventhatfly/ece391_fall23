#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "lib.h"
#define COLS    80  /* Number of columns in terminal */
#define ROWS    25  /* Number of rows in terminal */
#define ATTRIB  0x7 /* Attribute for terminal */
#define BUFFER_SIZE   128 /* Video memory address */
#define ENTER_ASC2	0xA /* Ascii code for enter */
/* the cursor coordinates is reserved for future use, now they are completely the same as the screen_x and screen_y */
typedef struct termin {
	unsigned char terminal_buffer[128];  /*define the buffer of 128 bytes according to the requirement*/
	uint32_t buffer_iterator;
	uint32_t  cursor_x_coord;          /* x_axis coordinate of cursor */
	uint32_t  cursor_y_coord;          /* y_axis coordinate of cursor */
} termin_t;

extern uint32_t terminal_clear();
extern void terminal_init();
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
extern uint32_t terminal_display(unsigned char input);
extern uint32_t terminal_delete();
extern uint32_t buffer_clear();
extern void draw_cursor(uint32_t x, uint32_t y);
extern int32_t terminal_open(const uint8_t *fname);
extern int32_t terminal_close(int32_t fd);

#endif 

