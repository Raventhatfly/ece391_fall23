#include "terminal.h"
#include "lib.h"

extern int screen_x;
extern int screen_y;
extern char* video_mem;  /* can the extern be used to link the variables in lib.c? */

static int32_t i;
termin_t my_terminal;

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
    for (i = 0; i < BUFFER_SIZE; i++) my_terminal.terminal_buffer[i] = ' '; /*clear the buffer of all the 128 chars*/
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
    if (screen_y < COLS-1){    /*if the row is not the last row(there are 25 rows so 24), go to the next row*/
        screen_y++;         
    }else{
        memcpy(video_mem, video_mem+COLS*2, COLS*(ROWS-1)*2); /* or copy the screen to the next line*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 ) = ' ';  /*clear the screen with blank*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 + 1) = ATTRIB;  /*set the ATTRIB of the screen*/
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
int32_t terminal_input(unsigned char input){
    if (input == ENTER_ASC2){             /* if the input is enter, clear the buffer and copy the screen as moving to next line*/
        my_terminal.buffer_iterator=buffer_clear();  
        screen_cpy();
        my_terminal.cursor_x_coord=screen_x;
        my_terminal.cursor_y_coord=screen_y;
        draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord); /*redraw the cursor*/
        return -1;
    }
    if (input == 0x0){
        return -1;
    }
    if (screen_x==79){      /*if the input is at the end(the 80th char) of the line, set the buffer as enter to change to next line*/
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = ENTER_ASC2; /*if the input is at the end of the line, set the buffer as enter to change to next line*/
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
void terminal_output(){
    printf("%c", my_terminal.terminal_buffer[my_terminal.buffer_iterator]); /*print the current char*/
    my_terminal.cursor_x_coord=screen_x;
    my_terminal.cursor_y_coord=screen_y;
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2) + 1) = ATTRIB; /*set the ATTRIB of the screen*/
    draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord); /*redraw the cursor*/
    my_terminal.buffer_iterator++;
}

/*
    * terminal_read
    *   DESCRIPTION: read the input from keyboard
    *   INPUTS: input -- the input from keyboard
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: read the input from keyboard
*/
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    return nbytes;  
}

/*
    * terminal_write
    *   DESCRIPTION: write the input to screen
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: write the input to screen
*/
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    return nbytes;
    /*my_terminal.buffer_iterator= my_terminal.buffer_iterator % BUFFER_SIZE; if the buffer is full, set the iterator to 0*/
}

/* The following cursor functions are developing from https://wiki.osdev.org/Text_Mode_Cursor */
/* the following two are added according to the document, but not used*/

/*
    * draw_cursor
    *   DESCRIPTION: update the cursor
    *   INPUTS: x -- the x position of the cursor
    *           y -- the y position of the cursor
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: update the cursor
*/
/*0x0F and Ox0E are the ports for the cursor*/
/*0x3D4 and 0x3D5 are the ports for the VGA board*/
/*0xFF is used to mask the high 8 bits*/
void draw_cursor(uint32_t x, uint32_t y) {	
	outb(0x0F, 0x3D4);						/* Tell the VGA board we are setting the high cursor byte. */
	outb((uint8_t) ( (COLS * y + x) & 0xFF), 0x3D5); 
	outb(0x0E, 0x3D4);						/* Tell the VGA board we are setting the low cursor byte. */
	outb((uint8_t) (((COLS * y + x) >> 8) & 0xFF), 0x3D5); 
}

/*the following two are useless functions for utliizing the syntax*/
/*
    * terminal_open
    *   DESCRIPTION: open the terminal, actually do nothing
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: open the terminal
*/
int32_t terminal_open(const uint8_t *fname){
    return 0;
}

/*
    * terminal_close
    *   DESCRIPTION: close the terminal, actually do nothing
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: close the terminal
*/
int32_t terminal_close(int32_t fd){
    return 0;
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
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2 + 1) = ATTRIB; /*set the ATTRIB of the screen*/
    i = 0;   
    screen_x = i;
    screen_y = i;
    my_terminal.cursor_x_coord=i;
    my_terminal.cursor_y_coord=i;
    i = buffer_clear();  /*clear the buffer*/
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
    if (my_terminal.buffer_iterator >= BUFFER_SIZE) return -1; /*if the buffer is full, return -1 to inform the failure*/
    flag = terminal_input(input);                    /*read the input, flag shows wthether succeed*/
    if (flag == -1) return flag;                    /*if the input is enter, return -1 to inform the failure*/
    terminal_output();                               /*if the input is not enter, write the input to screen*/
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
    if (screen_x == 0) {      /*if the input is at the beginning of the line, delete the last char of the last line*/
        screen_x = COLS - 1;
        screen_y--;
    }
    else {
        screen_x--;             /*or just delete the last char of the current line*/
    }
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2)) = ' '; /*initialize the char as blank and ATTRIB*/
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2) + 1) = ATTRIB;
    my_terminal.cursor_x_coord=screen_x;
    my_terminal.cursor_y_coord=screen_y;
    draw_cursor(my_terminal.cursor_x_coord, my_terminal.cursor_y_coord); /*redraw the cursor*/
    my_terminal.terminal_buffer[my_terminal.buffer_iterator] = ' '; 
    my_terminal.buffer_iterator--;
    my_terminal.buffer_iterator= my_terminal.buffer_iterator % BUFFER_SIZE; /*if the buffer is full, set the iterator to 0*/
    return 0;
}

