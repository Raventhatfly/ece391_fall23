
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#define KEYBOARD_PORT 0x60
#define KEYBOARD_IRQ 1
#define SCANCODE_SIZE 0x59
extern void key_init(void);

#endif
