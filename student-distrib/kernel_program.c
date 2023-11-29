#include "kernel_program.h"
#include "lib.h"
#include "syscall.h"
#include "page.h"
uint8_t* k_commands[K_COMMAND_NUM];
prog_function_ptr k_handlers[K_COMMAND_NUM];
int cmd_cnt = K_COMMAND_NUM;

void init_kernel_command(){
    k_commands[0] = "help";
    k_handlers[0] = (void*) help;
    k_commands[1] = "color";
    k_handlers[1] = (void*) color;
    k_commands[2] = "meminfo";
    k_handlers[2] = (void*) meminfo;
    
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

void color(int8_t* arg){
    int8_t* h = "-h";
    int i;
    int num;
    if(strncmp(arg, h, 2) == 0){
        printf("Color Table\n");
        printf("| code  |    color    |\n");
        printf("|   0   |    black    |\n");
        printf("|   1   |    blue     |\n");
        printf("|   2   |    green    |\n");
        printf("|   3   |    cyan     |\n");
        printf("|   4   |     red     |\n");
        printf("|   5   |   magenta   |\n");
        printf("|   6   |    brown    |\n");
        printf("|   7   |  light gray |\n");
        printf("|   8   |  dark gray  |\n");
        printf("|   9   |  light blue |\n");
        printf("|   a   | light green |\n");
        printf("|   b   |  light cyan |\n");
        printf("|   c   |  light red  |\n");
        printf("|   d   |light magenta|\n");
        printf("|   e   |    yellow   |\n");
        printf("|   f   |    white    |\n");
        return;
    }
    for(i=0;i<MAX_ARG_LEN;i++){
        if(arg[i]==' ' || arg[i] == '\0')   break;
    }
    if(i>1){
        printf("Please input argument as a single hedeximal number\n");
        return;
    }
    if('0'<=arg[0] && arg[0]<= '9'){
        if(arg[0] == '0'){
            printf("Warning: you are using the background color. \n");
        }
        num = (int)(arg[0] - '0');
    }else if('a'<= arg[0] && arg[0]<='f'){
        num = (int)(arg[0] - 'a' + 10);
    }else{
        printf("Invalid argument.\n");
        return;
    }
    set_attribute(num);
}

void meminfo(int8_t* arg){
    display_memory();
}
void calc(int8_t* arg){
    printf("This is a calculator!\n");
}
