#ifndef SYSCALL_H
#define SYSCALL_H
#include "types.h"
#include "scheduler.h"

#define MAX_CMD 32
// #define MAX_ARGS 10
#define MAX_ARG_LEN 32
#define MAX_PROCESS 6
#define KERNEL_STACK_ADDR 0x800000     /* 8 MB */
#define USER_PROGRAM_ADDR 0x8048000     
#define USER_STACK_ADDR   0x8400000     /* 132 MB */
#define PCB_SIZE 0x2000                 /* 8kB */

extern int32_t terminal_using;

typedef struct file_op_table{
    int32_t (*open)(const uint8_t *fname);
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*close)(int32_t fd);
}file_op_table_t;

file_op_table_t rtc_op_table;
file_op_table_t dir_op_table;
file_op_table_t file_op_table;
file_op_table_t stdin_op_table;
file_op_table_t stdout_op_table;

typedef struct file_desc{
    file_op_table_t* file_op_table_ptr;
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
}file_desc_t;

typedef struct pcb{
    file_desc_t file_desc_arr[8];
    int32_t pid;
    int32_t parent_pid;
    uint8_t cmd[MAX_CMD + 1];
    int32_t arg_cnt;
    uint8_t args[MAX_ARG_LEN + 1];
    int32_t esp;
    int32_t ebp;
    int32_t terminal_id;
}pcb_t;

int32_t allocate_pid();
pcb_t * fetch_pcb_addr(int32_t pid);
void push_pcb();

extern int32_t halt (uint8_t status);
extern int32_t execute (const uint8_t* command);
extern int32_t read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t write (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t open (const uint8_t* filename);
extern int32_t close (int32_t fd);
extern int32_t getargs (uint8_t* buf, int32_t nbytes);
extern int32_t vidmap (uint8_t** screen_start);
extern int32_t set_handler (int32_t signum, void* handler_address);
extern int32_t sigreturn (void);
extern void file_op_table_init();
int32_t fetch_curr_pid();
int null_read(int32_t fd, void* buf, int32_t nbytes);
int null_write(int32_t fd, const void* buf, int32_t nbytes);
#endif
