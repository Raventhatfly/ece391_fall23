#include "scheduler.h"
#include "syscall.h"
#include "x86_desc.h"
#include "page.h"
#include "terminal.h"
#include "lib.h"

int scheduler_activated = 0;
terminal_proc_t terminal_pid_map[TERMINAL_NUM];
int curr_exe_terminal;

/*
    * scheduler_init
    *   DESCRIPTION: initialize the scheduler
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/
void scheduler_init(){
    /* set up the scheduler */
    int i;
    for(i=0;i<TERMINAL_NUM;i++){
        terminal_pid_map[i].num_proc = 0;
        terminal_pid_map[i].pid = PID_EMPTY;
        curr_exe_terminal = 0;
    }
    scheduler_activated = 1;
}


/*
    * process_switch
    *   DESCRIPTION: swith to the next process
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: negative number on failure and 0 on success
    *   SIDE EFFECTS: change the process
*/
int process_switch(){
    /* check if scheduler is activated */
    if(scheduler_activated == 0){
        return -2;  /* return -2 if scheduler not initialized */
    }
    if(terminal_pid_map[0].pid == PID_EMPTY)    return -1;  /* if default terminal not executing, finish the program */
    
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
    cur_pcb->scheduler_ebp = cur_ebp;
    cur_pcb->scheduler_esp = cur_esp;

    int next_terminal;
    if(terminal_pid_map[0].pid == PID_EMPTY)    return -1;  /* if default terminal not executing, finish the program */
    for(i=1;i<TERMINAL_NUM;i++){
        if(terminal_pid_map[i].pid == PID_EMPTY && i==get_terminal_id()){
            set_mem(i);
            curr_exe_terminal = i;
            execute((uint8_t*)"shell");
        }
    }
    next_terminal = curr_exe_terminal;
    do{
        next_terminal = (next_terminal + 1) % TERMINAL_NUM;
        next_pid = terminal_pid_map[next_terminal].pid;
    }while(next_pid == PID_EMPTY); 
    curr_exe_terminal = next_terminal;
    
    pcb_t* next_pcb = fetch_pcb_addr(next_pid);
    /* User program remap */
    program_page_init(next_pid);
    //set_mem(next_pcb->terminal_id);
    set_mem(curr_exe_terminal);

    /* Context Switch */
    tss.ss0 = KERNEL_DS;
    tss.esp0 =  KERNEL_STACK_ADDR - next_pid * PCB_SIZE - 4;   /* Prevent stack collision. Using 4 as alignment *//* -4 alignment */

    asm volatile(
        "movl %0, %%esp\n"
        "movl %1, %%ebp\n"
        "leave\n"
        "ret\n"
        :
        : "r" (next_pcb->scheduler_esp), "r" (next_pcb->scheduler_ebp)
    );
    return 0;   /* prevent warning; never execute here */

}

int change_terminal_process(int pid, int terminal_id){
    terminal_pid_map[terminal_id].pid = pid;
    return 0;
}
