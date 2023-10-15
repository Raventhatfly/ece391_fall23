#ifndef IRQ_HANDLERS_H
#define IRQ_HANDLERS_H

/* Exception Handlers */


extern void exception0_handler();
extern void exception1_handler();
extern void exception2_handler();
extern void exception3_handler();
extern void exception4_handler();
extern void exception5_handler();
extern void exception6_handler();
extern void exception7_handler();
extern void exception8_handler();
extern void exception9_handler();
extern void exception10_handler();
extern void exception11_handler();
extern void exception12_handler();
extern void exception13_handler();
extern void exception14_handler();
extern void exception15_handler();
extern void exception16_handler();
extern void exception17_handler();
extern void exception18_handler();
extern void exception19_handler();


/* IRQ Handlers */
extern void irq0_handler();
extern void irq1_handler();         /* keyboard handler */
extern void irq2_handler();
extern void irq3_handler();
extern void irq4_handler();
extern void irq5_handler();
extern void irq6_handler();
extern void irq7_handler();
extern void rtc_handler();         /* rtc handler */
extern void irq9_handler(); 
// extern void rtc_irqhandler();   /* IRQ 8, RTC */
/* System Calls */
extern void syscall_handler();
#endif
