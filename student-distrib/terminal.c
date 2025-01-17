#include "terminal.h"
#include "lib.h"
#include "syscall.h"
#include "page.h"
extern int screen_x;
extern int screen_y;
extern char* video_mem;  

/*start shell flag */
termin_t my_terminal[TERMINAL_NUM];
static int32_t i;
int32_t terminal_using;
last_cmd_t prev_cmd[TERMINAL_NUM][1000]; //1000 is the max number of commands to store
int32_t curr_ask_cmd[TERMINAL_NUM]={0,0,0};
int32_t num_cmd[TERMINAL_NUM];
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
void set_mem(int32_t terminal_id)
{
    screen_x=my_terminal[terminal_id].cursor_x_coord;
    screen_y=my_terminal[terminal_id].cursor_y_coord;
    int32_t video_idx=VIDEO>>12;
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
    * terminal_input
    *   DESCRIPTION: read the input from keyboard
    *   INPUTS: input -- the input from keyboard
    *   OUTPUTS: none
    *   RETURN VALUE: -1 if the input is enter and nothing to print, 0 if the input is not enter
    *   SIDE EFFECTS: read the input from keyboard
*/
int32_t buffer_set(unsigned char input){
    if (input == ENTER_ASC2){     
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = ENTER_ASC2;
        my_terminal[terminal_using].buffer_iterator++;
        
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
        set_mem(terminal_using);//3.5
        putc_keyboard(ENTER_ASC2); /*print the current char*/
        set_mem(curr_exe_terminal);//3.5    
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = input; /*set the buffer as the input to display*/
                                               /*copy the screen as moving to next line*/
        return 0;
    }else{
        my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = input; /*set the buffer as the input to display*/
        return 0;
    }     
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
    if(terminal_id<0||terminal_id>2) return -1; /*if the terminal id is not valid, return -1 to inform the failure*/
    if(terminal_id==terminal_using) return 0; /*if the terminal is the current terminal, return 0 to inform the success*/

    my_terminal[terminal_id].terminal_active = 1;
    
    screen_x=my_terminal[terminal_id].cursor_x_coord;
    screen_y=my_terminal[terminal_id].cursor_y_coord; 

    set_mem(terminal_using);
    memcpy((void*)backup_hidden_terminal[terminal_using], (void*)video_mem, FOURKB); /*copy the video memory to the terminal buffer*/
    memcpy((void*)video_mem,(void*)backup_hidden_terminal[terminal_id], FOURKB);
    terminal_using=terminal_id;
    draw_cursor(my_terminal[terminal_id].cursor_x_coord, my_terminal[terminal_id].cursor_y_coord);
    set_mem(curr_exe_terminal);
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
    buffer_clear(curr_exe_terminal); 
    my_terminal[curr_exe_terminal].read_flag = 0; 
    while (!my_terminal[curr_exe_terminal].read_flag);  /*wait until the enter is pressed*/

    my_terminal[curr_exe_terminal].terminal_buffer[my_terminal[curr_exe_terminal].buffer_iterator-1] = '\0';

    prev_cmd[curr_exe_terminal][num_cmd[curr_exe_terminal]].cmd_len=my_terminal[curr_exe_terminal].buffer_iterator-2;
    for (i = 0; i < prev_cmd[curr_exe_terminal][num_cmd[curr_exe_terminal]].cmd_len; i++) 
        prev_cmd[curr_exe_terminal][num_cmd[curr_exe_terminal]].cmd[i] = my_terminal[curr_exe_terminal].terminal_buffer[i];
    prev_cmd[curr_exe_terminal][num_cmd[curr_exe_terminal]].cmd[i] = '\0';
    num_cmd[curr_exe_terminal]++;
    curr_ask_cmd[curr_exe_terminal]=num_cmd[curr_exe_terminal];
    if (num_cmd[curr_exe_terminal]==1000) num_cmd[curr_exe_terminal]=1000;//1000 is the max number of commands to store


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
        putc(*pointer); /*write the input to screen*/
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
    set_attribute(0x0F);
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2) = ' ';
    for (i = 0; i < ROWS * COLS; i++) *(uint32_t *)(video_mem + i*2 + 1) = get_attribute(); /*set the ATTRIB of the screen*/
    i = 0;   
    screen_x = i;
    screen_y = i;
    my_terminal[terminal_using].cursor_x_coord=0;
    my_terminal[terminal_using].cursor_y_coord=0;
    my_terminal[terminal_using].read_flag=0;
    my_terminal[terminal_using].terminal_flag=0;
    draw_cursor(my_terminal[i].cursor_x_coord, my_terminal[i].cursor_y_coord);
    
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
        if(i==0){
            my_terminal[i].terminal_active = 1;
        }else{
            my_terminal[i].terminal_active = 0;     /* set the active terminal value to not activated state */
        }
        buffer_clear(i);
    }
    for (i=0;i<BUFFER_SIZE;i++){
        prev_cmd[0][0].cmd[i]='\0';
        prev_cmd[1][0].cmd[i]='\0';
        prev_cmd[2][0].cmd[i]='\0';
    }
    prev_cmd[0][0].cmd_len=0;
    prev_cmd[1][0].cmd_len=0;
    prev_cmd[2][0].cmd_len=0;
    terminal_using=0;
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
    flag = buffer_set(input);                    /*read the input, flag shows wthether succeed*/
    if (flag == -1) return flag;                    /*if the input is enter, return -1 to inform the failure*/
    
    set_mem(terminal_using);
    putc_keyboard(my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator]); /*print the current char*/
    set_mem(curr_exe_terminal);//3.5                             /*if the input is not enter, write the input to screen*/
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
    *(uint32_t *)(video_mem + ((COLS * screen_y + screen_x) * 2) + 1) = get_attribute();
    my_terminal[terminal_using].cursor_x_coord=screen_x;
    my_terminal[terminal_using].cursor_y_coord=screen_y;
    draw_cursor(my_terminal[terminal_using].cursor_x_coord, my_terminal[terminal_using].cursor_y_coord); /*redraw the cursor*/
    my_terminal[terminal_using].terminal_buffer[my_terminal[terminal_using].buffer_iterator] = ' '; 
    my_terminal[terminal_using].buffer_iterator--;
    my_terminal[terminal_using].buffer_iterator= my_terminal[terminal_using].buffer_iterator % BUFFER_SIZE; /*if the buffer is full, set the iterator to 0*/
    
    return 0;
}

/*
    * get_terminal_id
    *   DESCRIPTION: get the id of the current using terminal
    *   INPUTS: none
    *   OUTPUTS: the ID of the current using terminal
    *   RETURN VALUE: 
    *   SIDE EFFECTS: none
*/
int32_t get_terminal_id(){
    return terminal_using;
}

/*
    * active_termminal_cnt
    *   DESCRIPTION: count the number of terminal that are currently active
    *   INPUTS: none
    *   OUTPUTS: the ID of the current using terminal
    *   RETURN VALUE: 
    *   SIDE EFFECTS: none
*/
int32_t active_termminal_cnt(){
    int i, cnt;
    cnt = 0;
    for(i = 0;i<TERMINAL_CNT;i++){
        if(my_terminal[i].terminal_active){
            cnt++;
        }
    }
    return cnt;
}
/*
    * show_last_cmd
    *   DESCRIPTION: show the last command
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 
    *   SIDE EFFECTS: none
*/
void show_last_cmd(){
    int i;
    while (1)
    {
        i=terminal_delete();
        if (i==-1) break;
    }
    if (curr_ask_cmd[terminal_using]>0) curr_ask_cmd[terminal_using]--;
    for (i=0;i<prev_cmd[terminal_using][curr_ask_cmd[terminal_using]].cmd_len;i++){
        terminal_display(prev_cmd[terminal_using][curr_ask_cmd[terminal_using]].cmd[i]);
    }
}
/*
    * show_next_cmd
    *   DESCRIPTION: show the next command
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: 
    *   SIDE EFFECTS: none
*/
void show_next_cmd(){
    int i;
    while (1)
    {
        i=terminal_delete();
        if (i==-1) break;
    }
    if (curr_ask_cmd[terminal_using]<num_cmd[terminal_using]) curr_ask_cmd[terminal_using]++;
    if (curr_ask_cmd[terminal_using]==num_cmd[terminal_using]) return;
    for (i=0;i<prev_cmd[terminal_using][curr_ask_cmd[terminal_using]].cmd_len;i++){
        terminal_display(prev_cmd[terminal_using][curr_ask_cmd[terminal_using]].cmd[i]);
    }
}
