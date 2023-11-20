#ifndef _SCHEDULER_H
#define _SCHEDULER_H

// #define MAX_PROCESS 6
#define MAX_TERMINAL_PROCESS 4
#define TERMINAL_CNT 3
#define SENTINEL_NODE 2
#define HEAD_NODE_PID -2
#define TAIL_NODE_PID -3
#define EMPTY_NODE_PID -4
#define HEAD_NODE 0
#define TAIL_NODE (SENTINEL_NODE + MAX_PROCESS - 1)

/* macros for the revised version */
#define PID_EMPTY -1
typedef struct terminal_proc{
    int num_proc;
    int pid;
}terminal_proc_t;

// extern int32_t terminal_using;
void scheduler_init();
int process_switch();
int change_terminal_process(int pid, int terminal_id);
#endif
