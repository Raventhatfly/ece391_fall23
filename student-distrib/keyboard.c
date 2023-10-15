#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
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
    /*print to screen*/
    /*
    key>=SCANCODE_SIZE means that the key is just a release
    */
    if (key<SCANCODE_SIZE) 
    {
        uint8_t ascii = scancode[key];
        putc(ascii);
    }
    /*send eoi*/
    send_eoi(KEYBOARD_IRQ);
    sti();
}
