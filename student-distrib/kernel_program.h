#ifndef _KERNEL_PROGRAM
#define _KERNEL_PROGRAM

#include "types.h"

#define K_COMMAND_NUM 2

typedef void (*prog_function_ptr)(int8_t* arg);
void init_kernel_command();
int excute_kernel_program(int8_t* cmd, int8_t *arg);
void* k_cmd_execute(int8_t *arg);


/* kernel programs */
void help(int8_t* arg);
void calc(int8_t* args);
#endif

