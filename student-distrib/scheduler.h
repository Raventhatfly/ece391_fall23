#ifndef _SCHEDULER_H
#define _SCHEDULER_H

// #define MAX_PROCESS 6
#define MAX_TERMINAL_PROCESS 3
#define TERMINAL_CNT 3
#define SENTINEL_NODE 2
#define HEAD_NODE_PID -2
#define TAIL_NODE_PID -3
#define EMPTY_NODE_PID -4
#define HEAD_NODE 0
#define TAIL_NODE (SENTINEL_NODE + MAX_PROCESS - 1)

typedef struct sch_proc{
    int pid;
    int terminal_id;
    int next_proc;
}sch_proc_t;

typedef struct terminal_proc{
    int num_proc;
}terminal_proc_t;

// extern int32_t terminal_using;
int process_switch();
int node_allocate();
int install_process(int pid, int terminal_id);
int remove_process(int pid, int terminal_id);
#endif
