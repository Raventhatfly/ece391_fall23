#define ASM 1
# #include "handler_dispatcher.h"

.text 
.global idt_exc_0, idt_exc_1, idt_exc_2, idt_exc_3, idt_exc_4,idt_exc_5,idt_exc_6
.global idt_exc_7, idt_exc_8, idt_exc_9, idt_exc_10, idt_exc_11, idt_exc_12, idt_exc_13
.global idt_exc_14, idt_exc_15, idt_exc_16, idt_exc_17, idt_exc_18, idt_exc_19

.global idt_irq_0, idt_irq_1, idt_irq_2, idt_irq_3, idt_irq_4, idt_irq_5
.global idt_irq_6, idt_irq_7, idt_irq_8
.global idt_syscall


idt_exc_0:
    pushal
    cld
    call exception0_handler
    popal
    iret 

idt_exc_1:
    pushal
    cld
    call exception1_handler
    popal 
    iret 

idt_exc_2:
    pushal
    cld
    call exception2_handler
    iret

idt_exc_3:
    pushal
    cld
    call exception3_handler
    popal
    iret

idt_exc_4:
    pushal
    cld
    call exception4_handler
    popal
    iret

idt_exc_5:
    pushal
    cld
    call exception5_handler
    popal
    iret

idt_exc_6:
    pushal
    cld
    call exception6_handler
    popal
    iret

idt_exc_7:
    pushal
    cld
    call exception7_handler
    popal
    iret

idt_exc_8:
    pushal
    cld
    call exception8_handler
    popal
    iret

idt_exc_9:
    pushal
    cld
    call exception9_handler
    popal
    iret

idt_exc_10:
    pushal
    cld
    call exception10_handler
    popal
    iret

idt_exc_11:
    pushal
    cld
    call exception11_handler
    popal
    iret

idt_exc_12:
    pushal
    cld
    call exception12_handler
    popal
    iret

idt_exc_13:
    pushal
    cld
    call exception13_handler
    popal
    iret

idt_exc_14:
    pushal
    cld
    call exception14_handler
    popal
    iret

idt_exc_15:
    pushal
    cld
    call exception15_handler
    popal
    iret

idt_exc_16:
    pushal
    cld
    call exception16_handler
    popal
    iret

idt_exc_17:
    pushal
    cld
    call exception17_handler
    popal
    iret

idt_exc_18:
    pushal
    cld
    call exception18_handler
    popal
    iret

idt_exc_19:
    pushal
    cld
    call exception19_handler
    popal
    iret

idt_irq_0:
    pushal
    cld
    call irq0_handler
    popal
    iret

idt_irq_1:
    pushal
    cld
    call irq1_handler
    popal
    iret

idt_irq_2:
    pushal
    cld
    call irq2_handler
    popal
    iret

idt_irq_3:
    pushal
    cld
    call irq3_handler
    popal
    iret

idt_irq_4:
    pushal
    cld
    call irq4_handler
    popal
    iret

idt_irq_5:
    pushal
    cld
    call irq5_handler
    popal
    iret

idt_irq_6:
    pushal
    cld
    call irq6_handler
    popal
    iret

idt_irq_7:
    pushal
    cld
    call irq7_handler
    popal
    iret

idt_irq_8:
    pushal
    cld
    call rtc_handler
    popal
    iret

idt_syscall:
    pushal
    cld
    
    # check the syscall number. Unsighed number must not be bigger than 10
    decl %eax
    cmpl $9, %eax
    ja .invalid_call 

    pushl %edx
    pushl %ecx
    pushl %ebx
    call *idt_syscall_jmp_table(,%eax,4)
    movl %eax, .ret_val
    addl $12, %esp

    .invalid_call:
    popal
    movl .ret_val, %eax
    iret

    .ret_val:
    .long 0

idt_syscall_jmp_table:
    .long halt
    .long execute
    .long read
    .long write
    .long open
    .long close
    .long getargs
    .long vidmap
    .long set_handler
    .long sigreturn
