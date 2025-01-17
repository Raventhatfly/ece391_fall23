#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "lib.h"
#define COLS    80  /* Number of columns in terminal */
#define ROWS    25  /* Number of rows in terminal */
#define BUFFER_SIZE   128 /* Video memory address */
#define ENTER_ASC2	0xA /* Ascii code for enter */
#define FAILURE 0
#define TERMINAL_NUM 3 /* Number of terminals */
#define FOURKB 4096 /* 4KB */
/* the cursor coordinates is reserved for future use, now they are completely the same as the screen_x and screen_y */
typedef struct termin {
	unsigned char terminal_buffer[128];  /*define the buffer of 128 bytes according to the requirement*/
	uint32_t buffer_iterator;
	uint32_t  cursor_x_coord;          /* x_axis coordinate of cursor */
	uint32_t  cursor_y_coord;          /* y_axis coordinate of cursor */
	int32_t read_flag;
	int32_t terminal_flag;
	int32_t terminal_active;
} termin_t;
typedef struct last_cmd {
	uint8_t cmd[128];
	uint32_t cmd_len;
} last_cmd_t;
extern uint32_t terminal_clear();
extern void terminal_init();
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
extern uint32_t terminal_display(unsigned char input);
extern uint32_t terminal_delete();
extern void draw_cursor(uint32_t x, uint32_t y);
extern int32_t terminal_open(const uint8_t *fname);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_switch(int32_t terminal_id);
extern int32_t get_terminal_id();
extern void set_mem(int32_t terminal_id);
extern int32_t active_termminal_cnt();
extern void show_last_cmd();
extern void show_next_cmd();

extern termin_t my_terminal[TERMINAL_NUM];
#endif 

