#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "terminal.h"
#include "scheduler.h"
#include "signal.h"
/*
table from scancode to ascii
*/
#define lctrl_press 0x1D
#define lctrl_release 0x9D
#define lshift_press 0x2A
#define lshift_release 0xAA
#define rshift_press 0x36
#define rshift_release 0xB6
#define caps_press 0x3A
#define caps_release 0xBA
#define alt_press 0x38
#define alt_release 0xB8
#define f1_press 0x3B
#define f2_press 0x3C
#define f3_press 0x3D
#define f4_press 0x3E
#define f5_press 0x3F
#define f6_press 0x40
#define f7_press 0x41
#define f8_press 0x42
#define f9_press 0x43
#define f10_press 0x44
#define f11_press 0x57
#define f12_press 0x58
#define Advances_code 0xE0
#define up_make 0x49
#define down_make 0x51
#define left_make 0x47
#define right_make 0x4F
#define up_break 0xC9
#define down_break 0xD1
#define left_break 0xC7
#define right_break 0xCF
/*
flag for special key
*/
int8_t caps=0,shift=0,ctrl=0,alt=0,advance=0;
extern int curr_exe_terminal;
/*
table from scancode to ascii
*/
uint8_t scancode[SCANCODE_SIZE]={
    0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,0,
    0,0,0,0,0,0,0,0,0,0,'-',0,0,0,0,0,
    0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',
    '2','3','0','.',0,0};
uint8_t scancode_shift[SCANCODE_SIZE]={
    0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','\"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',0,0,
    0,0,0,0,0,0,0,0,0,0,'_',0,0,0,0,0,
    0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',
    '2','3','0','.',0,0};
uint8_t scancode_caps[SCANCODE_SIZE]={
    0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','[',']','\n',
    0,'A','S','D','F','G','H','J','K','L',';','\'','`',0,'\\',
    'Z','X','C','V','B','N','M',',','.','/',0,'*',0,' ',0,0,
    0,0,0,0,0,0,0,0,0,0,'-',0,0,0,0,0,
    0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',
    '2','3','0','.',0,0};
uint8_t scancode_shiftcaps[SCANCODE_SIZE]={
    0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','{','}','\n',
    0,'a','s','d','f','g','h','j','k','l',':','\"','~',0,'|',
    'z','x','c','v','b','n','m','<','>','?',0,'*',0,' ',0,0,
    0,0,0,0,0,0,0,0,0,0,'_',0,0,0,0,0,
    0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',
    '2','3','0','.',0,0};
/*
key_init
input: none
output: none
side effect: enable the keyboard irq
*/
void key_init(void) 
{
	/*keyboard is irq1*/
	enable_irq(KEYBOARD_IRQ);
}
/*
irq1_handler
input: none
output: none
side effect: print the key to screen
*/
void irq1_handler(void)
{
    cli();
    /*read from keyboard*/
    uint8_t key = inb(KEYBOARD_PORT);
    /*
    change the special key to the flag
    */
    switch (key)
    {
        case lctrl_press:
            ctrl=1;
            break;
        case lctrl_release:
            ctrl=0;
            break;
        case lshift_press:
            shift=1;
            break;
        case rshift_press:
            shift=1;
            break;
        case lshift_release:
            shift=0;
            break;
        case rshift_release:
            shift=0;
            break;
        case caps_press:    
            caps=caps^1;
            break;
        case alt_press:
            alt=1;
            break;
        case alt_release:
            alt=0;
            break;
        case f1_press:
            if (alt==1)
            {
                terminal_switch(0);
                break;
            }
            break;
        case f2_press:
            if (alt==1)
            {
                terminal_switch(1);
                break;
            }
            break;
        case f3_press:
            if (alt==1)
            {
                terminal_switch(2);
                break;
            }
            break;
        case Advances_code:
            advance=advance^1;
            break;
    default:
        if (key<SCANCODE_SIZE)  /*if the key is not a special key and is the pressing process*/
        {
            uint8_t ascii;
            /*
            if shift is pressed, use the scancode_shift table
            */
            if (advance==1)
            {
                if (key==up_make) show_last_cmd();
                break;
            }
            if (shift==1 && caps==1) //if shift and caps are pressed
                ascii = scancode_shiftcaps[key];
            else if (shift==1) //if shift is pressed
                ascii = scancode_shift[key];
            /*
            if caps is pressed, use the scancode_caps table
            */
            else if (caps==1) //if caps is set
                ascii = scancode_caps[key];
            else
                ascii = scancode[key];
            if (ctrl && (ascii=='l' || ascii=='L'))
            {
                set_mem(get_terminal_id());
                terminal_clear();
                set_mem(curr_exe_terminal);
                break; 
            } else if(ctrl && (ascii=='c' || ascii=='C')){
                signal_trigger(INTERRUPT);
            } else if (ascii=='\b'){
                set_mem(get_terminal_id());
                terminal_delete();
                set_mem(curr_exe_terminal);
                break;
            }else if (ascii=='\t'){
                terminal_display(' ');
                terminal_display(' ');
            } else {
                terminal_display(ascii);
                break;
            }
            //putc(ascii);
            break;
        }
    }
    /*print to screen*/
    /*
    key>=SCANCODE_SIZE means that the key is just a release
    */
    /*send eoi*/
    send_eoi(KEYBOARD_IRQ);
    sti();
}
