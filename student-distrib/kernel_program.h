#ifndef _KERNEL_PROGRAM
#define _KERNEL_PROGRAM

#include "types.h"
#include "syscall.h"

#define K_COMMAND_NUM 6
#define MAX_ARGS 10


typedef void (*prog_function_ptr)(int8_t* arg);
void init_kernel_command();
int excute_kernel_program(int8_t* cmd, int8_t *arg);
void* k_cmd_execute(int8_t *arg);
void separate_args(int8_t *arg, int8_t args[MAX_ARGS][MAX_ARG_LEN + 1]);

/* kernel programs */
void help(int8_t* arg);
void color(int8_t* arg);
void meminfo(int8_t* arg);
void calc(int8_t* args);
void info(int8_t* info);
void serial(int8_t* arg);

#endif

