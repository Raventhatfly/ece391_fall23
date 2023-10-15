#include "handlers.h"
void exception0_handler(){
    printf("Exception: divided by 0!\n");
    while(1);
}
void exception1_handler(){
    printf("Exception: Debug. \n");
    while(1);
}
void exception2_handler(){
    printf("Exception: NMI interrupt. \n");
    while(1);
}
void exception3_handler(){
    printf("Exception: Breakpoint. \n");
    while(1);
}
void exception4_handler(){
    printf("Exception: Overflow ocuured! \n");
    while(1);
}
void exception5_handler(){
    printf("Exception: Bound range exceeded! \n");
    while(1);
}
void exception6_handler(){
    printf("Exception: Invalid Opcode! \n");
    while(1);
}
void exception7_handler(){
    printf("Exception: Device not available! \n");
    while(1);
}
void exception8_handler(){
    printf("Exception: Double fault! \n");
    while(1);
}
void exception9_handler(){
    printf("Exception: Coprocessor Segment overrun! \n");
    while(1);
}
void exception10_handler(){
    printf("Exception: Invalid TSS! \n");
    while(1);
}
void exception11_handler(){
    printf("Exception: Overflow ocuured! \n");
    while(1);
}
void exception12_handler(){
    printf("Exception: Stack segment fault! \n");
    while(1);
}
void exception13_handler(){
    printf("Exception: Gereral Protection. \n");
    while(1);
}
void exception14_handler(){
    printf("Exception: Page Fault! \n");
    while(1);
}
void exception15_handler(){
    printf("Eception 15. \n");
    while(1);
}
void exception16_handler(){
    printf("Eception: Floating point error! \n");
    while(1);
}
void exception17_handler(){
    printf("Eception: Aligment check. \n");
    while(1);
}
void exception18_handler(){
    printf("Eception: Machine check. \n");
    while(1);
}
void exception19_handler(){
    printf("Eception: SIMD floating-point exception. \n");
    while(1);
}

void irq0_handler(){return;}
// void irq1_handler(){return;}   
void irq2_handler(){return;}   
void irq3_handler(){return;}
void irq4_handler(){return;}
void irq5_handler(){return;}
void irq6_handler(){return;}
void irq7_handler(){return;}
// void irq8_handler(){return;}
void irq9_handler(){return;}
// /* System Calls */
void syscall_handler(){return;}
