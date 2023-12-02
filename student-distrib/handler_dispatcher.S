#define ASM 1
# #include "handler_dispatcher.h"

.text 
.global idt_exc_0, idt_exc_1, idt_exc_2, idt_exc_3, idt_exc_4,idt_exc_5,idt_exc_6
.global idt_exc_7, idt_exc_8, idt_exc_9, idt_exc_10, idt_exc_11, idt_exc_12, idt_exc_13
.global idt_exc_14, idt_exc_15, idt_exc_16, idt_exc_17, idt_exc_18, idt_exc_19

.global idt_irq_0, idt_irq_1, idt_irq_2, idt_irq_3, idt_irq_4, idt_irq_5
.global idt_irq_6, idt_irq_7, idt_irq_8
.global idt_syscall

# Exception 0 ~ 7, 9, 15, 16, 18, 19 have no error code 
idt_exc_0:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception0_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret 

idt_exc_1:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception1_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret 

idt_exc_2:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception2_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_3:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception3_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_4:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception4_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_5:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception5_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_6:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception6_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_7:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception7_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_8:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception8_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp    # Pop IRQ Exception Code
    addl $4, %esp   # Pop Error Code 
    iret

idt_exc_9:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception9_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_10:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception10_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp    # Pop IRQ Exception Code
    addl $4, %esp   # Pop Error Code 
    iret

idt_exc_11:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception11_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4, %esp   # Pop Error Code 
    iret

idt_exc_12:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception12_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp    # Pop IRQ Exception Code
    addl $4, %esp   # Pop Error Code 
    iret

idt_exc_13:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception13_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp # Pop Error Code 
    iret

idt_exc_14:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception14_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp    # pop error code
    iret

idt_exc_15:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception15_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_16:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception16_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_17:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception17_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp # Pop Error Code 
    iret

idt_exc_18:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception18_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_exc_19:
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call exception19_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    iret

idt_irq_0:
    addl $-4,%esp
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call pit_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_1:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call irq1_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_2:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call irq2_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_3:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call serial_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_4:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call serial_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_5:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call irq5_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_6:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call irq6_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_7:
    addl $-4,%esp   # Error Code
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call irq7_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_irq_8:
    addl $-4,%esp
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    call rtc_handler
    call signal_handler
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4,%esp
    iret

idt_syscall:
    addl $-4, %esp
    addl $-4, %esp  # IRQ Exception Code
    pushl %fs 
    pushl %es
    pushl %ds 
    pushl %eax
    pushl %ebp
    pushl %edi 
    pushl %esi 
    pushl %edx 
    pushl %ecx 
    pushl %ebx
    cld
    
    # check the syscall number. Unsighed number must not be bigger than 10
    decl %eax
    cmpl $12, %eax          # Number of system calls minus one
    ja .invalid_call 

    pushl %edx
    pushl %ecx
    pushl %ebx
    call *idt_syscall_jmp_table(,%eax,4)
    movl %eax, .ret_val
    addl $12, %esp

    .invalid_return:
    popl %ebx
    popl %ecx 
    popl %edx 
    popl %esi 
    popl %edi 
    popl %ebp 
    popl %eax  
    popl %ds 
    popl %es 
    popl %fs 
    addl $4,%esp # Pop IRQ Exception Code
    addl $4, %esp
    movl .ret_val, %eax
    iret

    # the call is invalid, fill in the error code -1
    .invalid_call:
    movl $-1, .ret_val
    jmp .invalid_return

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
    .long user_malloc
    .long user_free
    .long set_color
