/* rtc.c - Functions to interact with RTC */

#include "rtc.h"
#include "lib.h"
#include "i8259.h"

/*
    *  void rtc_init()
    *    DESCRIPTION: initialize the rtc
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: - Configures the RTC control registers A and B to enable periodic 
    *                   interrupts and set the interrupt rate.
    *                 - Enables the RTC IRQ line on the PIC (Programmable Interrupt Controller),
    *                   allowing the system to receive and handle RTC interrupts.
*/
void rtc_init(void) {
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

    enable_irq(RTC_IRQ);              /* Enable RTC interrupt */
}

/*
    *  void rtc_handler()
    *    DESCRIPTION: Handles the RTC (Real-Time Clock) interrupt by reading 
    *                 from the RTC to clear the interrupt flag, setting a flag 
    *                 to indicate an interrupt has occurred, and sending an 
    *                 End Of Interrupt (EOI) signal to the Programmable 
    *                 Interrupt Controller (PIC).
    *    INPUT: none
    *    OUTPUT: none
    *    SIDE EFFECT: 
    *                 - Sends an End Of Interrupt (EOI) signal to the PIC.
    *                 - Temporarily disables and then re-enables interrupts to 
    *                   ensure atomic operations.
*/
void rtc_handler(void){
    /* disable interrupt */
    cli();
    outb(REG_C, REG_SELECT);
    inb(REG_DATA);
    test_interrupts();
    send_eoi(RTC_IRQ);              /* End Of Interrupt (EOI) signal */
    sti();                          /* re-enable interrupt */
}
