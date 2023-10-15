
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#define KEYBOARD_PORT 0x60
#define KEYBOARD_IRQ 1 /*keyboard's irq number*/
#define SCANCODE_SIZE 99 /*0x59 is the largest scancode for pressing*/
extern void key_init(void);

#endif
