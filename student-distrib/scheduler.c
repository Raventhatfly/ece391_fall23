#include "scheduler.h"
#include "syscall.h"
#include "x86_desc.h"
#include "page.h"
#include "terminal.h"
#include "lib.h"

int scheduler_activated = 0;
terminal_proc_t terminal_process_mapping[3];
/* array implemented linked list */
sch_proc_t active_proc_list[MAX_PROCESS + SENTINEL_NODE];
/* modified version */
int terminal_pid_map[TERMINAL_NUM];
int curr_exe_terminal;

void scheduler_init(){
    /* set up the scheduler */
    int j;
    int i = HEAD_NODE + 1;
    for(i=0;i<TERMINAL_NUM;i++){
        terminal_process_mapping[i].num_proc = 0;
        terminal_pid_map[i] = PID_EMPTY;
        curr_exe_terminal = 0;
    }
    scheduler_activated = 1;
}


/* return -2 if scheduler not initialized */
int process_switch(){
    /* check if scheduler is activated */
    if(scheduler_activated == 0){
        return -2;  /* return -2 if scheduler not initialized */
    }

    int cur_esp;
    int cur_ebp;
    int next_pid;
    int i;
    int cur_pid = fetch_curr_pid();
    pcb_t* cur_pcb = fetch_pcb_addr(cur_pid);
    asm volatile(
        "movl %%esp, %0\n"
        "movl %%ebp, %1\n"
        : "=r" (cur_esp), "=r" (cur_ebp)
    );
    cur_pcb->ebp = cur_ebp;
    cur_pcb->esp = cur_esp;

    /* start */
    int next_terminal;
    if(terminal_pid_map[0] == PID_EMPTY)    return -1;  /* if default terminal not executing, finish the program */
    for(i=1;i<TERMINAL_NUM;i++){
        if(terminal_pid_map[i] == PID_EMPTY && i==get_terminal_id()){
            set_mem(i);
            execute("shell");
        }
    }
    next_terminal = curr_exe_terminal;
    do{
        next_terminal = (next_terminal + 1) % TERMINAL_NUM;
        next_pid = terminal_pid_map[next_terminal];
    }while(next_pid == PID_EMPTY && next_terminal != 0);
    curr_exe_terminal = next_terminal;
    /* end */

    
    pcb_t* next_pcb = fetch_pcb_addr(next_pid);
    /* User program remap */
    program_page_init(next_pid);
    set_mem(next_pcb->terminal_id);

    /* Context Switch */
    tss.ss0 = KERNEL_DS;
    tss.esp0 =  KERNEL_STACK_ADDR - next_pid * PCB_SIZE - 4;   /* Prevent stack collision. Using 4 as alignment *//* -4 alignment */

    asm volatile(
        "movl %0, %%esp\n"
        "movl %1, %%ebp\n"
        "leave\n"
        "ret\n"
        :
        : "r" (next_pcb->esp), "r" (next_pcb->ebp)
    );
}

int change_terminal_process(int pid, int terminal_id){
    terminal_pid_map[terminal_id] = pid;
    return 0;
}
