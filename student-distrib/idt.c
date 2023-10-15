#include "idt.h"
#include "handler_dispatcher.h"
void idt_init(){
    /* set idt exception */
    SET_IDT_ENTRY(idt[0],idt_exc_0);
    SET_IDT_ENTRY(idt[1],idt_exc_1);
    SET_IDT_ENTRY(idt[2],idt_exc_2);
    SET_IDT_ENTRY(idt[3],idt_exc_3);
    SET_IDT_ENTRY(idt[4],idt_exc_4);
    SET_IDT_ENTRY(idt[5],idt_exc_5);
    SET_IDT_ENTRY(idt[6],idt_exc_6);
    SET_IDT_ENTRY(idt[7],idt_exc_7);
    SET_IDT_ENTRY(idt[8],idt_exc_8);
    SET_IDT_ENTRY(idt[9],idt_exc_9);
    SET_IDT_ENTRY(idt[10],idt_exc_10);
    SET_IDT_ENTRY(idt[11],idt_exc_11);
    SET_IDT_ENTRY(idt[12],idt_exc_12);
    SET_IDT_ENTRY(idt[13],idt_exc_13);
    SET_IDT_ENTRY(idt[14],idt_exc_14);
    SET_IDT_ENTRY(idt[15],idt_exc_15);
    SET_IDT_ENTRY(idt[16],idt_exc_16);
    SET_IDT_ENTRY(idt[17],idt_exc_17);
    SET_IDT_ENTRY(idt[18],idt_exc_18);
    SET_IDT_ENTRY(idt[19],idt_exc_19);

    
    /* set idt interrupts */
    SET_IDT_ENTRY(idt[IRQ_START],idt_irq_0);
    SET_IDT_ENTRY(idt[IRQ_START+1],idt_irq_1);
    SET_IDT_ENTRY(idt[IRQ_START+2],idt_irq_2);
    SET_IDT_ENTRY(idt[IRQ_START+3],idt_irq_3);
    SET_IDT_ENTRY(idt[IRQ_START+4],idt_irq_4);
    SET_IDT_ENTRY(idt[IRQ_START+5],idt_irq_5);
    SET_IDT_ENTRY(idt[IRQ_START+6],idt_irq_6);
    SET_IDT_ENTRY(idt[IRQ_START+7],idt_irq_7);
    SET_IDT_ENTRY(idt[IRQ_START+8],idt_irq_8);
    

    /* set idt system call */
    SET_IDT_ENTRY(idt[SYSCALL_START],idt_syscall);

    /* Set remaining bits of IDT */
    /* 
        0b1110 or 0xE: 32-bit Interrupt Gate
        0b1111 or 0xF: 32-bit Trap Gate
        Reseverd3 is the Least Significant bit 
    */
    int i = 0;
    for(i=0;i<IDT_SIZE;i++){
        /* TODO: do not set everything to present */
        idt[i].present = 1;                                                                          
        idt[i].reserved1 = 1;                                             
        idt[i].reserved2 = 1;                                                                                     
        idt[i].present = 1;                                                
        idt[i].size = 1;     
        idt[i].seg_selector = KERNEL_CS;
        if(i<IRQ_START){      
            idt[i].dpl = 0;             /* set to kernnel privellege level 0*/
            idt[i].reserved3 = 1;       /* Trap */
        }else if(i==0x80){
            idt[i].dpl = 3;             /* set to user privellege level 3*/
            idt[i].reserved3 = 1;       /* Trap */      
        }else{
            idt[i].dpl = 0;
            idt[i].reserved3 = 0;       /* Interrupt */
        }

    }
}