#include "signal.h"
#include "lib.h"
#include "scheduler.h"
#include "x86_desc.h"
#include "terminal.h"

/*
    * signal_init
    *   DESCRIPTION: Signal Initialization by setting the default value
    *   INPUTS: cur_pcb
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/
void signal_init(pcb_t* cur_pcb){
    /* set to default value */
    int i;
    cur_pcb->sig_hand[0] = signal_kill;
    cur_pcb->sig_hand[1] = signal_kill;
    cur_pcb->sig_hand[2] = signal_kill;
    cur_pcb->sig_hand[3] = signal_ignore;
    cur_pcb->sig_hand[4] = signal_ignore;
    for(i=0;i<SIG_NUM;i++){
        cur_pcb->sig_state[i] = SIG_IDLE;
    }
    signal_unmask(cur_pcb);
}

/*
    * signal_handler
    *   DESCRIPTION: handle the signal 
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: change to the stack
*/
void signal_handler(){
    int32_t i,flag;
    int32_t cur_ebp, user_esp;
    int32_t sig_num = SIG_NONE;
    int32_t pid;
    pcb_t* cur_pcb;
    hw_context_t* context_addr;
    
    function_ptr func;
    user_function_ptr user_func;
    asm volatile(
        "movl %%ebp, %0\n"
        : "=r" (cur_ebp)
    );
    context_addr = (hw_context_t*)(cur_ebp + 8);     /* the distance from the current ebp to context stack frame */ 
    /* 1: Mask all other signals */
    pid = fetch_curr_pid();
    cur_pcb = fetch_pcb_addr(pid);
    cli_and_save(flag);
    for(i=0;i<SIG_NUM;i++){
        if(cur_pcb->sig_state[i] == SIG_PENDING && signal_check_mask(cur_pcb, i)){
            sig_num = i;
            cur_pcb->sig_state[i] = SIG_IDLE;
            signal_mask(cur_pcb, i);
            break;
        }
    }
    restore_flags(flag);
    if(sig_num == SIG_NONE)     return;               /* No signal */
    func = (void*) cur_pcb->sig_hand[sig_num];  
    /* if using default handlers */
    if(func == signal_kill || func == signal_ignore ){
        func();
        signal_unmask(cur_pcb);
        return;
    }else if(sig_num == ALARM){
        user_func = (user_function_ptr)func;
        user_func(sig_num);
        signal_unmask(cur_pcb);
        return;
    }        
    

    /* 2: Set up the signal handler’s stack frame*/
    user_esp = context_addr->stored_esp;
    
    if(context_addr->stored_cs != USER_CS)  return;     /* only deliver the signal when returning to user*/
    int sig_code_size = ((int)sig_code_end - (int)sig_code_start);
    int32_t return_addr = user_esp - sig_code_size;               
    
    /* fill in the return address onto the stack */
    int32_t* ret_loc = (int32_t*)(user_esp - sig_code_size - HARDWARE_CONTEXT_SIZE - 8);
    *ret_loc = return_addr;
    /* fill in the signal number onto the stack */
    int32_t* sig_num_loc = (int32_t*)(user_esp - sig_code_size - HARDWARE_CONTEXT_SIZE - 4);
    *sig_num_loc = sig_num;
    /* copy the hardware context onto the stack */
    memcpy((void*)(user_esp - sig_code_size - HARDWARE_CONTEXT_SIZE),context_addr,HARDWARE_CONTEXT_SIZE);
    /* copy the return code onto the stack */
    memcpy((void*)(user_esp - sig_code_size), (void*)sig_code_start, sig_code_size);

    context_addr->stored_esp = user_esp - sig_code_size - HARDWARE_CONTEXT_SIZE - 8;
    context_addr->return_addr = (int32_t)func;
}

/*
    * signal_kill
    *   DESCRIPTION: null read
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: killing the process. The process might not return
*/
void signal_kill(){
    int pid;
    pcb_t* pcb;
    pid = fetch_curr_pid();
    pcb = fetch_pcb_addr(pid);
    signal_unmask(pcb);
    halt(0);
}

/*
    * signal_trigger
    *   DESCRIPTION: none
    *   INPUTS: signal number
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/
void signal_trigger(int signal){
    int pid, terminal_id, flag;
    pcb_t* pcb;
    cli_and_save(flag);
    terminal_id = get_terminal_id();
    if(signal == INTERRUPT){
        pcb = fetch_pcb_addr(terminal_pid_map[terminal_id].pid);
    }else{
        pid = fetch_curr_pid();
        pcb = fetch_pcb_addr(pid);
    }
    
    pcb->sig_state[signal] = SIG_PENDING;
    restore_flags(flag);
}

/*
    * signal_ignore
    *   DESCRIPTION: unmask the igored signal
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/
void signal_ignore(){
    int pid;
    pcb_t* pcb;
    pid = fetch_curr_pid();
    pcb = fetch_pcb_addr(pid);
    signal_unmask(pcb);
}

/*
    * signal_check_mask
    *   DESCRIPTION: null read
    *   INPUTS: pcb, sig_num
    *   OUTPUTS: none
    *   RETURN VALUE: positive when mask is set, 0 on not set
    *   SIDE EFFECTS: none
*/
int32_t signal_check_mask(pcb_t* pcb, int32_t sig_num){
    return pcb->sig_mask & (1 << sig_num);
}
void signal_mask(pcb_t* pcb, int32_t sig_num){
    pcb->sig_mask = 0;
    pcb->sig_mask = 1 << sig_num; 
}

/*
    * signal_unmask
    *   DESCRIPTION: unmasking all the signals
    *   INPUTS: none
    *   OUTPUTS: none
    *   RETURN VALUE: none
    *   SIDE EFFECTS: none
*/
void signal_unmask(pcb_t* pcb){
    pcb->sig_mask = 0x1f;
}
