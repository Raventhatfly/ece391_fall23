#include "handlers.h"
#include "syscall.h"
/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception of divided by 0
    *  INPUT/OUTPUT: none
*/
void exception0_handler(){
    printf("Exception: divided by 0!\n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Debug
    *  INPUT/OUTPUT: none
*/
void exception1_handler(){
    printf("Exception: Debug. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: NMI interrupt
    *  INPUT/OUTPUT: none
*/
void exception2_handler(){
    printf("Exception: NMI interrupt. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Breakpoint
    *  INPUT/OUTPUT: none
*/
void exception3_handler(){
    printf("Exception: Breakpoint. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Overflow ocuured
    *  INPUT/OUTPUT: none
*/
void exception4_handler(){
    printf("Exception: Overflow ocuured! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Bound range exceeded
    *  INPUT/OUTPUT: none
*/
void exception5_handler(){
    printf("Exception: Bound range exceeded! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Invalid Opcode
    *  INPUT/OUTPUT: none
*/
void exception6_handler(){
    printf("Exception: Invalid Opcode! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Device not available
    *  INPUT/OUTPUT: none
*/
void exception7_handler(){
    printf("Exception: Device not available! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Double fault
    *  INPUT/OUTPUT: none
*/
void exception8_handler(){
    printf("Exception: Double fault! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Coprocessor Segment overrun
    *  INPUT/OUTPUT: none
*/
void exception9_handler(){
    printf("Exception: Coprocessor Segment overrun! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Invalid TSS
    *  INPUT/OUTPUT: none
*/
void exception10_handler(){
    printf("Exception: Invalid TSS! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Overflow ocuured
    *  INPUT/OUTPUT: none
*/
void exception11_handler(){
    printf("Exception: Overflow ocuured! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Stack segment fault
    *  INPUT/OUTPUT: none
*/
void exception12_handler(){
    printf("Exception: Stack segment fault! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Gereral Protection
    *  INPUT/OUTPUT: none
*/
void exception13_handler(){
    printf("Exception: Gereral Protection. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Exception: Page Fault
    *  INPUT/OUTPUT: none
*/
void exception14_handler(){
    printf("Exception: Page Fault! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Eception 15
    *  INPUT/OUTPUT: none
*/
void exception15_handler(){
    printf("Eception 15. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Eception: Floating point error
    *  INPUT/OUTPUT: none
*/
void exception16_handler(){
    printf("Eception: Floating point error! \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Eception: Aligment check
    *  INPUT/OUTPUT: none
*/
void exception17_handler(){
    printf("Eception: Aligment check. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Eception: Machine check
    *  INPUT/OUTPUT: none
*/
void exception18_handler(){
    printf("Eception: Machine check. \n");
    halt(-1);
    while(1);
}

/*
    *  void exception0_handler()
    *  DESCRIPTION: Eception: SIMD floating-point exception
    *  INPUT/OUTPUT: none
*/
void exception19_handler(){
    printf("Eception: SIMD floating-point exception. \n");
    halt(-1);
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

/*
    *  void syscall_handler()
    *  DESCRIPTION: Eception: SIMD floating-point exception
    *  INPUT/OUTPUT: none
*/
void syscall_handler(){
    printf("System Call!\n");
}
