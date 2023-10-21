#include "terminal.h"
#include "lib.h"
//#include "lib.c" 
//with some problems


extern int screen_x;
extern int screen_y;
extern char* video_mem;

static int32_t i;
termin_t my_terminal;

uint32_t buffer_clear(){
    for (i = 0; i < 128; i++) my_terminal.terminal_buffer[i] = ' '; //maybe with some problems
    my_terminal.buffer_iterator = 0;
    return 0;
}

void screen_cpy(){
    screen_x = 0;
    if (screen_y < 24){
        screen_y++;
    }else{
        memcpy(video_mem, video_mem+COLS*2, COLS*(ROWS-1)*2);
        for (i = 0; i < COLS; i++) *(uint8_t *)(video_mem + COLS*(ROWS-1)*2 + (i << 1)) = ' ';
        for (i = 0; i < COLS; i++) *(uint8_t *)(video_mem + COLS*(ROWS-1)*2 + (i << 1) + 1) = 0x7;   
    }
}

uint32_t terminal_clear(){
    for (i = 0; i < ROWS * COLS; i++) *(uint8_t *)(video_mem + (i << 1)) = ' ';
    for (i = 0; i < ROWS * COLS; i++) *(uint8_t *)(video_mem + (i << 1) + 1) = 0x7;
    i = 0;    //maybe with some problems
    screen_x = i;
    screen_y = i;
    i = buffer_clear(); //maybe with some problems
    return 0;
}

uint32_t terminal_read(unsigned char input){
    if (input == 0xA){
        my_terminal.buffer_iterator=buffer_clear();
        screen_cpy();
        return -1;
    }
    if (screen_x==79){
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = 0xA;
        my_terminal.buffer_iterator++;
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = input;
        screen_cpy();
        return 0;
    }else{
        my_terminal.terminal_buffer[my_terminal.buffer_iterator] = input;
        return 0;
    }     
}

void terminal_write(){
    printf("%c", my_terminal.terminal_buffer[my_terminal.buffer_iterator]);
    my_terminal.buffer_iterator++;
}

uint32_t terminal_display(unsigned char input){
    int flag;
    if (my_terminal.buffer_iterator >= 128) return -1;
    flag = terminal_read(input);
    if (flag == -1) return flag;
    terminal_write();
    return 0;
}

uint32_t terminal_delete(){
    if (screen_x == 0) {
        if(screen_y == 0) {
            return -1;
        }
    }
    if (my_terminal.buffer_iterator = 0){
        return -1;
    }
    if (screen_x == 0) {
        screen_x = COLS - 1;
        screen_y--;
    }
    else {
        screen_x--;
    }
    *(uint8_t *)(video_mem + ((COLS * screen_y + screen_x) << 1)) = ' ';
    *(uint8_t *)(video_mem + ((COLS * screen_y + screen_x) << 1) + 1) = 0x7;
    my_terminal.buffer_iterator--;
    return 0;
}




