#include "terminal.h"
#include "lib.h"
#include "syscall.h"
#include "page.h"
extern int screen_x;
extern int screen_y;
extern char* video_mem;  

/*start shell flag */
term_shell_flag[TERMINAL_NUM] = {0};
termin_t my_terminal[TERMINAL_NUM];
static int32_t i;
int32_t terminal_using;

extern int curr_exe_terminal;
uint32_t* backup_hidden_terminal[3]={(uint32_t*)0xB9000, (uint32_t*)0xBA000, (uint32_t*)0xBB000};
/* the following four function are helper functions */
/*
    * buffer_clear
    *   DESCRIPTION: clear the buffer
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 0
    *   SIDE EFFECTS: clear the buffer
*/
uint32_t buffer_clear(int32_t id){
    for (i = 0; i < BUFFER_SIZE; i++) my_terminal[id].terminal_buffer[i] = ' '; /*clear the buffer of all the 128 chars*/
    my_terminal[id].buffer_iterator = 0;
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
    if (screen_y < ROWS-1){    /*if the row is not the last row(there are 25 rows so 24), go to the next row*/
        screen_y++;         
    }else{
        memcpy(video_mem, video_mem+COLS*2, COLS*(ROWS-1)*2); /* or copy the screen to the next line*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 ) = ' ';  /*clear the screen with blank*/
        for (i = 0; i < COLS; i++) *(uint32_t *)(video_mem + (COLS*(ROWS-1)+i)*2 + 1) = ATTRIB;  /*set the ATTRIB of the screen*/
    }
}

/*
    * terminal_input
    *   DESCRIPTION: read the input from keyboard
    *   INPUTS: input -- the input from keyboard
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: read the input from keyboard
*/
int32_t terminal_input(unsigned char input){
    if (input == ENTER_ASC2){     
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = ENTER_ASC2;
        my_terminal[terminal_using].buffer_iterator++;
        
        // screen_cpy();
        // my_terminal[terminal_using].cursor_x_coord=screen_x;
        // my_terminal[terminal_using].cursor_y_coord=screen_y;
        // draw_cursor(my_terminal[terminal_using].cursor_x_coord, my_terminal[terminal_using].cursor_y_coord); /*redraw the cursor*/
        set_mem(terminal_using);//3.5
        putc_keyboard(ENTER_ASC2); /*print the current char*/
        set_mem(curr_exe_terminal);//3.5    
        my_terminal[terminal_using].read_flag=1;
        return -1;
    }
    if (input == 0x0){
        return -1;
    }
    if (screen_x==79){      /*if the input is at the end(the 80th char) of the line, set the buffer as enter to change to next line*/
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = ENTER_ASC2; /*if the input is at the end of the line, set the buffer as enter to change to next line*/
        //my_terminal[terminal_using].buffer_iterator++;
        set_mem(terminal_using);//3.5
        putc_keyboard(ENTER_ASC2); /*print the current char*/
        set_mem(curr_exe_terminal);//3.5    
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = input; /*set the buffer as the input to display*/
                                               /*copy the screen as moving to next line*/
        //my_terminal[terminal_using].buffer_iterator++;
        return 0;
    }else{
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = input; /*set the buffer as the input to display*/
        //my_terminal[terminal_using].buffer_iterator++;
        return 0;
    }     
}

/*
    * terminal_output
    *   DESCRIPTION: write the input to screen
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: write the input to screen
*/
void terminal_output(){
    //int temp;
    set_mem(terminal_using);//3.5
    //temp=curr_exe_terminal;
    //curr_exe_terminal=terminal_using;
    putc_keyboard(my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator]); /*print the current char*/
    //curr_exe_terminal=temp;
    //my_terminal[terminal_using].cursor_x_coord=screen_x;
    //my_terminal[terminal_using].cursor_y_coord=screen_y;
   
    //pcb_t* cur_pcb = fetch_pcb_addr(fetch_curr_pid()); //3.5
    //set_mem(cur_pcb->terminal_id);//3.5
    set_mem(curr_exe_terminal);//3.5
}

void set_mem(int32_t terminal_id)
{
    screen_x=my_terminal[terminal_id].cursor_x_coord;
    screen_y=my_terminal[terminal_id].cursor_y_coord;
    int32_t video_idx=VIDEO>>12;
    pcb_t* cur_pcb = fetch_pcb_addr(fetch_curr_pid());
    if (terminal_id==terminal_using)
    {
        
        page_table_entries[video_idx]=(page_table_entries[video_idx] & 0x00000fff) | VIDEO; /* set the video memory*/
        video_table_entries[video_idx]=(video_table_entries[video_idx] & 0x00000fff) | VIDEO; /* set the video memory*/
    }
    else
    {
        page_table_entries[video_idx]=(page_table_entries[video_idx] & 0x00000fff) | (uint32_t)backup_hidden_terminal[terminal_id]; /* set the video memory*/
        video_table_entries[video_idx]=(video_table_entries[video_idx] & 0x00000fff) | (uint32_t)backup_hidden_terminal[terminal_id]; /* set the video memory*/
    }
    uint32_t page_dir_addr = (uint32_t) &page_directory_entries; /* update CR3 register to use the new page directory and flush the TLB*/
    asm volatile("              \n\
    movl %0,%%eax               \n\
    movl %%eax,%%cr3            \n\
    "
    :
    : "r" (page_dir_addr)
    : "eax");
}
/*
    * terminal_switch
    *   DESCRIPTION: switch the terminal
    *   INPUTS: terminal_id -- the id of the terminal to switch to
    *   OUTPUTS: none
    *   RETURN VALUE: 0 if succeed, -1 if failed
    *   SIDE EFFECTS: switch the terminal
*/
int32_t terminal_switch(int32_t terminal_id){
    //int32_t flag;
    if(terminal_id<0||terminal_id>2) return -1; /*if the terminal id is not valid, return -1 to inform the failure*/
    if(terminal_id==terminal_using) return 0; /*if the terminal is the current terminal, return 0 to inform the success*/
    
    //cli_and_save(flag);
    screen_x=my_terminal[terminal_id].cursor_x_coord;
    screen_y=my_terminal[terminal_id].cursor_y_coord; //maybe some problems
    draw_cursor(my_terminal[terminal_id].cursor_x_coord, my_terminal[terminal_id].cursor_y_coord);

    set_mem(terminal_using);
    memcpy((void*)backup_hidden_terminal[terminal_using], (void*)video_mem, FOURKB); /*copy the video memory to the terminal buffer*/
    memcpy((void*)video_mem,(void*)backup_hidden_terminal[terminal_id], FOURKB);
    terminal_using=terminal_id;
    //pcb_t* cur_pcb = fetch_pcb_addr(fetch_curr_pid());
    //set_mem(cur_pcb->terminal_id);
    set_mem(curr_exe_terminal);
    //restore_flags(flag);
    return 0;
}   

/*
    * terminal_read
    *   DESCRIPTION: read the input from keyboard
    *   INPUTS: fd -- file descriptor, ignored
    *           buf -- buffer to read into
    *           nbytes -- number of bytes to read
    *   OUTPUTS: none
    *   RETURN VALUE: number of bytes read
    *   SIDE EFFECTS: read the input from keyboard
*/
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int32_t i = 0;
    int32_t j = 0;
	if (buf == NULL) return -1;  /*if the buffer is NULL or the nbytes is 0 or negative, return 0 as failed*/
    if (nbytes <= 0) return -1;  
    my_terminal[curr_exe_terminal].read_flag=0;
	// my_terminal.buffer_iterator = 0;	
    buffer_clear(curr_exe_terminal); 
    my_terminal[curr_exe_terminal].read_flag = 0; 
    while (!my_terminal[curr_exe_terminal].read_flag);  /*wait until the enter is pressed*/
    my_terminal[curr_exe_terminal].terminal_buffer[my_terminal[curr_exe_terminal].buffer_iterator-1] = '\0';
    if (nbytes > BUFFER_SIZE) {     /*if the nbytes is larger than the buffer size, set the j as the buffer size*/
        j = BUFFER_SIZE;
    }else{
        j = nbytes;
    }									
	for (i = 0; i < j && my_terminal[curr_exe_terminal].terminal_buffer[i] != '\0'; i++) ((char*)buf)[i] = my_terminal[terminal_using].terminal_buffer[i];
	return i;
}

/*
    * terminal_write
    *   DESCRIPTION: write the input to screen
    *  INPUTS: fd -- file descriptor, ignored
    *         buf -- buffer to write from
    *        nbytes -- number of bytes to write
    * OUTPUTS: none
    * RETURN VALUE: number of bytes written
    * SIDE EFFECTS: write the input to screen
*/
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes) {
    if (buf == NULL) return 0; /*if the buffer is NULL or the nbytes is 0 or negative, return 0 as failed*/
    if (nbytes <= 0) return 0;
    const char* pointer = (const char*)buf;
    int32_t char_written = 0; 
    /* Write nbytes bytes of buf to the terminal */
    while (char_written < nbytes && *pointer != '\0') { 
        putc(*pointer);
        //my_terminal[curr_exe_terminal].cursor_x_coord=screen_x;
        //my_terminal[curr_exe_terminal].cursor_y_coord=screen_y;
        pointer++;
        char_written++;
    }
    return char_written;
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
    //int32_t flag;
    //cli_and_save(flag);
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2) = ' ';
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2 + 1) = ATTRIB; /*set the ATTRIB of the screen*/
    i = 0;   
    screen_x = i;
    screen_y = i;
    my_terminal[terminal_using].cursor_x_coord=0;
    my_terminal[terminal_using].cursor_y_coord=0;
    my_terminal[terminal_using].read_flag=0;
    my_terminal[terminal_using].terminal_flag=0;
    draw_cursor(my_terminal[i].cursor_x_coord, my_terminal[i].cursor_y_coord);
    
    //restore_flags(flag);
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
    for (i=0;i<TERMINAL_NUM;i++){
        terminal_using=i;
        terminal_clear();      /*clear the terminal*/
    }
    for (i=0;i<TERMINAL_NUM;i++) buffer_clear(i);
    terminal_using=0;
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
    /* Ensure that the last character in the buffer is \n or \r */
    if(my_terminal[terminal_using].buffer_iterator == BUFFER_SIZE -1 && (!(input == '\n' || input == '\r'))) return -1; 
    /* if the buffer is full, return -1 to inform the failure */
    if (my_terminal[terminal_using].buffer_iterator >= BUFFER_SIZE) return -1; 
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
    /* 
     * if the position is on the shell ending pos 7 and the former bytes contain '>' on screen_x == 5 
     * we assume the content before this location is the shell prompt and it cannot be deleted.
     */
    //int flag;
    //cli_and_save(flag);

    if (screen_x == 7 && *(video_mem + ((COLS * screen_y + 5) * 2))=='>'){  
        return -1;
    }
    if(my_terminal[terminal_using].buffer_iterator == 0){   /* if the buffer is empty, do not delete */
        return -1;
    }
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
    my_terminal[terminal_using].cursor_x_coord=screen_x;
    my_terminal[terminal_using].cursor_y_coord=screen_y;
    draw_cursor(my_terminal[terminal_using].cursor_x_coord, my_terminal[terminal_using].cursor_y_coord); /*redraw the cursor*/
    my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = ' '; 
    my_terminal[terminal_using].buffer_iterator--;
    my_terminal[terminal_using].buffer_iterator= my_terminal[terminal_using].buffer_iterator % BUFFER_SIZE; /*if the buffer is full, set the iterator to 0*/
    
    //restore_flags(flag);
    return 0;
}

/* Return terminal Id */
int32_t get_terminal_id(){
    return terminal_using;
}
