#ifndef HANDLER_DISPATCHER_H
#define HANDLER_DISPATCHER_H


// /* Exception Wrappers */
extern void idt_exc_0(void);
extern void idt_exc_1(void);
extern void idt_exc_2(void);
extern void idt_exc_3(void);
extern void idt_exc_4(void);
extern void idt_exc_5(void);
extern void idt_exc_6(void);
extern void idt_exc_7(void);
extern void idt_exc_8(void);
extern void idt_exc_9(void);
extern void idt_exc_10(void);
extern void idt_exc_11(void);
extern void idt_exc_12(void);
extern void idt_exc_13(void);
extern void idt_exc_14(void);
extern void idt_exc_15(void);
extern void idt_exc_16(void);
extern void idt_exc_17(void);
extern void idt_exc_18(void);
extern void idt_exc_19(void);

/* IRQ Wrappers */
extern void idt_irq_0(void);
extern void idt_irq_1(void);
extern void idt_irq_2(void);
extern void idt_irq_3(void);
extern void idt_irq_4(void);
extern void idt_irq_5(void);
extern void idt_irq_6(void);
extern void idt_irq_7(void);
extern void idt_irq_8(void);


/* Syscall Wrappr */
extern void idt_syscall(void);
#endif
