#ifndef SYSCALL_H
#define SYSCALL_H
#include "types.h"

#define MAX_CMD 32
#define MAX_ARGS 10
#define MAX_ARG_LEN 32

/* commands and args */
uint8_t cmd[MAX_CMD + 1] = {'\0'};
uint8_t args[MAX_ARGS][MAX_ARG_LEN + 1] = {'\0'};

typedef struct file_desc{
    uint32_t file_op_table_ptr;
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
}file_desc_t;

typedef struct pcb{
    file_desc_t file_desc_arr[8];
}pcb_t;

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

#endif