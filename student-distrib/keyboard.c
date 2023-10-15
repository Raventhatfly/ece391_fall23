#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
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
/*
flag for special key
*/
int8_t caps=0,shift=0,ctrl=0,alt=0;
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
void key_init(void) 
{
	/*keyboard is irq1*/
	enable_irq(KEYBOARD_IRQ);
}
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
    default:
        if (key<SCANCODE_SIZE)  /*if the key is not a special key*/
        {
            uint8_t ascii;
            /*
            if shift is pressed, use the scancode_shift table
            */
            if (shift==1)
                ascii = scancode_shift[key];
            /*
            if caps is pressed, use the scancode_caps table
            */
            else if (caps==1)
                ascii = scancode_caps[key];
            else
                ascii = scancode[key];
            putc(ascii);
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
