#include "kernel_program.h"
#include "lib.h"
uint8_t* k_commands[K_COMMAND_NUM];
prog_function_ptr k_handlers[K_COMMAND_NUM];
int cmd_cnt = K_COMMAND_NUM;

void init_kernel_command(){
    k_commands[0] = "help";
    k_handlers[0] = (void*) help;
    k_commands[1] = "calc";
    k_handlers[1] = (void*) calc;
    
}

int excute_kernel_program(int8_t* cmd, int8_t *arg){
    int i,len, ret;
    prog_function_ptr func;
    ret = 0;
    for(i=0;i<K_COMMAND_NUM;i++){
        len = strlen(k_commands[i]);
        if(strncmp(cmd,k_commands[i],len) == 0){
            func = k_handlers[i];
            func(arg);
            ret = 1;
        }
    }
    return ret;
}

void help(int8_t* arg){
    printf("Welcome to RazelOS!\n");
    printf("The OS support three terminals, 6 user programs and %d kernel programs.\n", cmd_cnt);
    printf("Support User Commands:\n");
    printf("-- ls --: to show all the file/user_programs in the file system\n");
    printf("Support Kernel Commands:\n");
    printf("-- help --: get help and usage from the RazelOS\n");
    printf("-- calc --: OS support calculator\n");
}

void calc(int8_t* arg){
    printf("This is a calculator!\n");
}
