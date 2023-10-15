#ifndef IDT_H
#define IDT_H
#include "x86_desc.h"


#define IRQ_START 0x20
#define SYSCALL_START 0x80
#define IDT_SIZE 256
extern void idt_init();


#endif
