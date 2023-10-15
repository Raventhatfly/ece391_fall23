#include "handlers.h"
void exception0_handler(){
    printf("Exception: divided by 0!\n");
    handler_dead();
}
void exception1_handler(){
    printf("Exception: NMI interrupt. \n");
    handler_dead();
}
void exception2_handler(){
    printf("Exception: Breakpoint. \n");
    handler_dead();
}
void exception3_handler(){
    printf("Exception: Overflow ocuured! \n");
    handler_dead();
}
void exception4_handler(){return;}
void exception5_handler(){return;}
void exception6_handler(){return;}
void exception7_handler(){return;}
void exception8_handler(){return;}
void exception9_handler(){return;}
void exception10_handler(){return;}
void exception11_handler(){return;}
void exception12_handler(){return;}
void exception13_handler(){return;}
void exception14_handler(){
    printf("Exception: Page Fault! \n");
    // handler_dead();
}
void exception15_handler(){return;}
void exception16_handler(){return;}
void exception17_handler(){return;}
void exception18_handler(){return;}
void exception19_handler(){return;}

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

void handler_dead(){
    while(1);
}