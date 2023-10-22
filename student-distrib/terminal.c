#include "terminal.h"
#include "lib.h"

extern int screen_x;
extern int screen_y;
extern char* video_mem;  /* can the extern be used to link the variables in lib.c? */

static int32_t i;
termin_t my_terminal;

/*maybe the uint32_t * should be uint8*/
/* the following four function are helper functions */
/*
    * buffer_clear
    *   DESCRIPTION: clear the buffer
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: clear the buffer
*/
uint32_t buffer_clear(){
    for (i = 0; i < 128; i++) my_terminal.terminal_buffer[i] = ' '; /*this sentence may cause bugs*/
    my_terminal.buffer_iterator = 0;
    return 0;
}

/*
    * screen_cpy
    *   DESCRIPTION: copy the screen by video memory to next line
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: copy the screen
*/
void screen_cpy(){
    screen_x = 0;          /*set the position of the row*/
    if (screen_y < 24){    /*if the row is not the last row(there are 25 rows so 24), go to the next row*/
        screen_y++;         
    }else{
        memcpy(video_mem, video_mem+COLS*2, COLS*(ROWS-1)*2); /* or copy the screen to the next line*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 ) = ' ';  /*clear the screen with blank*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 + 1) = 0x7;  /*set the ATTRIB of the screen, use 0x7 as it can't be directly link to lib.c*/
    }
}

/*
    * terminal_read
    *   DESCRIPTION: read the input from keyboard
    *   INPUTS: input -- the input from keyboard
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: read the input from keyboard
*/
uint32_t terminal_read(unsigned char input){
    if (input == 0xA){             /* if the input is enter, clear the buffer and copy the screen as moving to next line*/
        my_terminal.buffer_iterator=buffer_clear();  
        screen_cpy();
        return -1;
    }
    if (screen_x==79){
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = 0xA; /*if the input is at the end of the line, set the buffer as enter to change to next line*/
        my_terminal.buffer_iterator++;
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = input; /*set the buffer as the input to display*/
        screen_cpy();                                               /*copy the screen as moving to next line*/
        return 0;
    }else{
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = input; /*set the buffer as the input to display*/
        return 0;
    }     
}

/*
    * terminal_write
    *   DESCRIPTION: write the input to screen
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: write the input to screen
*/
void terminal_write(){
    printf("%c", my_terminal.terminal_buffer[my_terminal.buffer_iterator]); /*print the current char*/
    draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord); /*redraw the cursor*/
    my_terminal.buffer_iterator++;
}

/* The following cursor functions are developing from https://wiki.osdev.org/Text_Mode_Cursor */

/*
    * vga_enable_cursor
    *   DESCRIPTION: enable the cursor
    *   INPUTS: cursor_start -- the start position of the cursor
    *           cursor_end -- the end position of the cursor
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: enable the cursor
*/
/*maybe should be uint8*/
void enable_cursor(uint32_t cursor_start, uint32_t cursor_end) {
	outb(0x0A, 0x3D4);					
	outb((inb(0x3D5) & 0xC0) | cursor_start, 0x3D5);	
	outb(0x0B, 0x3D4);						
	outb((inb(0x3D5) & 0xE0) | cursor_end, 0x3D5);		
}


/*
    * vga_disable_cursor
    *   DESCRIPTION: disable the cursor
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: disable the cursor
*/
void disable_cursor() {
	outb(0x0A, 0x3D4);						
	outb(0x20, 0x3D5);						
}


/*
    * vga_update_cursor
    *   DESCRIPTION: update the cursor
    *   INPUTS: x -- the x position of the cursor
    *           y -- the y position of the cursor
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: update the cursor
*/
void draw_cursor(uint32_t x, uint32_t y) {	
    /*uint16_t pos = y * COLS + x;*/	
	outb(0x0F, 0x3D4);						
	outb((uint8_t) ( (COLS * y + x) & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);						
	outb((uint8_t) (((COLS * y + x) >> 8) & 0xFF), 0x3D5);
}


/* the following functions are functional functions */
/*
    * terminal_clear
    *   DESCRIPTION: clear the terminal
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: clear the terminal
*/
uint32_t terminal_clear(){
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2) = ' ';
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2 + 1) = 0x7; 
    i = 0;    //maybe with some problems
    screen_x = i;
    screen_y = i;
    my_terminal.cursor_x_coord=i;
    my_terminal.cursor_y_coord=i;
    i = buffer_clear(); //maybe with some problems
    /*enable_cursor(0, 14);  maybe some problems */
    draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord);
    return 0;
}

/*
    * terminal_init
    *   DESCRIPTION: initialize the terminal by terminal_clear(). This function is used to utilize the syntax.
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: initialize the terminal
*/
void terminal_init(){
    i = terminal_clear();
    return;
}

/*
    * terminal_display
    *   DESCRIPTION: display the input to screen
    *   INPUTS: input -- the input from keyboard
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: display the input to screen
*/
uint32_t terminal_display(unsigned char input){
    int flag;
    if (my_terminal.buffer_iterator >= 128) return -1; /*if the buffer is full, return -1 to inform the failure*/
    flag = terminal_read(input);                    /*read the input, flag shows wthether succeed*/
    if (flag == -1) return flag;                    /*if the input is enter, return -1 to inform the failure*/
    terminal_write();                               /*if the input is not enter, write the input to screen*/
    return 0;
}

/*
    * terminal_delete
    *   DESCRIPTION: delete the input from screen
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: delete the input from screen
*/
uint32_t terminal_delete(){
    if (screen_x == 0) {     /*if the input is at the beginning of the line, delete is failed*/
        if(screen_y == 0) {
            return -1;
        }
    }
    if (my_terminal.buffer_iterator == 0){ /*if the buffer is empty, delete is failed*/
        return -1;
    }
    if (screen_x == 0) {      /*if the input is at the beginning of the line, delete the last char of the last line*/
        screen_x = COLS - 1;
        screen_y--;
    }
    else {
        screen_x--;             /*or just delete the last char of the current line*/
    }
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2)) = ' '; /*initialize the char as blank and ATTRIB*/
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2) + 1) = 0x7;
    my_terminal.cursor_x_coord=screen_x;
    my_terminal.cursor_y_coord=screen_y;
    draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord); /*redraw the cursor*/
    /*my_terminal.terminal_buffer[my_terminal.buffer_iterator] = ' '; initialize the buffer as blank*/
    my_terminal.buffer_iterator--;
    return 0;
}

