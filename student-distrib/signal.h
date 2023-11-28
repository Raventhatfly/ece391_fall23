#ifndef _SIGNAL_H
#define _SIGNAL_H
#include "syscall.h"
#define SIG_NUM 5

extern void sig_code_start(void);
extern void sig_code_end(void);
extern void get_sig_return_code(void*start, void *end);
extern int curr_exe_terminal;

#define SIG_RETURN_CODE_SIZE (SIG_RETURN_END - SIG_RETURN_START)
#define HARDWARE_CONTEXT_SIZE   72

#define SIG_NONE -1
#define SIG_IDLE 0
#define SIG_PENDING 1

#define DIV ZER 0
#define SEGFAULT 1 
#define INTERRUPT 2
#define ALARM 3
#define USER1 4

typedef void (*function_ptr)(void);
typedef void (*user_function_ptr)(int num);
typedef struct pushed_registers{
    int32_t ebx;
    int32_t ecx;
    int32_t edx;
    int32_t esi;
    int32_t edi;
    int32_t ebp;
    int32_t eax;
    int32_t ds;
    int32_t es;
    int32_t fs;
}register_t;

typedef struct hardware_context{
    register_t registers;
    int32_t irq_exc;
    int32_t error_code;
    int32_t return_addr;
    int32_t stored_cs;
    int32_t stored_eflags;
    int32_t stored_esp;
    int32_t stored_ss;
}hw_context_t;

void signal_init(pcb_t* cur_pcb);
void signal_handler();
void signal_kill();
void signal_ignore();
void signal_trigger(int signal);
int32_t signal_check_mask(pcb_t* pcb, int32_t sig_num);
void signal_mask(pcb_t* pcb, int32_t sig_num);
void signal_unmask(pcb_t* pcb);
#endif
