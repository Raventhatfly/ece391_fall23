/* rtc.c - Functions to interact with RTC */

#include "rtc.h"
#include "lib.h"
#include "i8259.h"

void rtc_init() {
    char prev;
    outb(REG_B,REG_SELECT);
    prev = inb(REG_DATA);
    outb(REG_B,REG_SELECT);
    outb(prev | 0x40,REG_DATA);       /* Enable the Periodic Interrupt */
    
    outb(REG_A,REG_SELECT);
    prev = inb(REG_DATA);
    outb(REG_A,REG_SELECT);
    
    /* Set the interrupt rate while preserving the upper 4 bits of Register A */
    outb((prev & 0xF0) | RTC_RATE, REG_DATA);
}

void rtc_handler(){
    /* disable interrupt */
	cli();
    outb(REG_C, REG_SELECT);
    inb(REG_DATA);
    INT_FLAG = 1;                   /* RTC interrupt has occurred */
    test_interrupts();
    send_eoi(RTC_IRQ);              /* End Of Interrupt (EOI) signal */
	sti();                          /* re-enable interrupt */
}

