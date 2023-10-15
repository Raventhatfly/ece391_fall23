/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */
/*
    * void i8259_init(void)
    * Inputs: none
    * Return Value: none
    * Function: Initialize the 8259 PIC
*/
/* Initialize the 8259 PIC */
void i8259_init(void) {
    /* initialize the mask*/
    master_mask=0xFF;
    slave_mask=0xFF;

    outb(master_mask, MASTER_8259_DATA); /*mask all interrupt on master */
    outb(slave_mask, SLAVE_8259_DATA); /* mask all interrupts on slave */

    /*send the ICW to the PICs*/
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_DATA);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);
    outb(ICW3_MASTER, MASTER_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA);
    outb(ICW4, MASTER_8259_DATA);
    outb(ICW4, SLAVE_8259_DATA);

    enable_irq(SLAVE_IRQ); /*enable slave*/
}
/*
    * void enable_irq(uint32_t irq_num)
    * Inputs: irq_num - the irq number
    * Return Value: none
    * Function: Enable (unmask) the specified IRQ
    *
*/
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) 
{
    /*unmask mask*/
    /*if master*/
    if (irq_num<8)  
    {
        master_mask&=(~(1<<irq_num));
        outb(master_mask,MASTER_8259_DATA);
        /*change te mask and send it to the data port*/
    } 
    /*if slave*/
    else if (irq_num<16) 
    {
        slave_mask&=(~(1<<(irq_num-8)));
        outb(slave_mask,SLAVE_8259_DATA);
        /*change te mask and send it to the data port*/
    }
}
/*
    * void disable_irq(uint32_t irq_num)
    * Inputs: irq_num - the irq number
    * Return Value: none
    * Function: Disable (mask) the specified IRQ
    *
*/
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) 
{
    /*mask mask*/
    /*if master*/
    if (irq_num<8) 
    {
        master_mask|=(1<<irq_num);
        outb(master_mask,MASTER_8259_DATA);
        /*change te mask and send it to the data port*/
    } 
    /*if slave*/
    else if (irq_num<16) 
    {
        slave_mask|=(1<<(irq_num-8));
        outb(slave_mask,SLAVE_8259_DATA);
        /*change te mask and send it to the data port*/
    }
}
/*
    * void send_eoi(uint32_t irq_num)
    * Inputs: irq_num - the irq number
    * Return Value: none
    * Function: Send end-of-interrupt signal for the specified IRQ
    *
*/
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) 
{
    if (irq_num<8)
    {
        /*send the EOI to master*/
        outb(EOI|irq_num,MASTER_8259_PORT);
    }
    else if (irq_num<16)
    {
        /*send the EOI to slave*/
        outb(EOI|(irq_num-8),SLAVE_8259_PORT);
        /*send the EOI to master*/
        outb(EOI|SLAVE_IRQ,MASTER_8259_PORT);
    }
}
