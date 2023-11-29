#include "kernel_program.h"
#include "lib.h"
#include "syscall.h"
#include "page.h"
#include "calculator.h"
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
    k_commands[3] = "calc";
    k_handlers[3] = (void*) calc;
    k_commands[4] = "info";
    k_handlers[4] = (void*) info;
    
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
    printf("The OS support three terminals, 9 user programs(3 terminal shells and extra shells above them) and %d kernel programs.\n", cmd_cnt);
    printf("Support User Commands:\n");
    printf("-- ls --: to show all the file/user_programs in the file system\n");
    printf("Support Kernel Commands:\n");
    printf("-- help --: get help and usage from the RazelOS\n");
    printf("-- color --: change the color of the characters shown on the screen\n");
    printf("-- meminfo --: show memory allocation information\n");
    printf("-- calc --: RazelOS support calculator\n");
    printf("-- info --: Operating System information\n");
    printf("Key support:\n");
    printf("Ctrl+L: clear screen\n");
    printf("Alt+F1/F2/F3: change terminal\n");
    printf("Ctrl+C: Halt current process\n");
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
    int len, answer;
    len = strlen(arg);
    answer =  cal((char*)arg, len);
    printf("%d\n",answer);
}

void info(int8_t* arg){
    char att = get_attribute();
    set_attribute(0x0b);    /* color light cyan */
    printf(" /$$$$$$$                                /$$                    \n");
    printf("| $$__  $$                              | $$                    \n");
    printf("| $$  \\ $$  /$$$$$$  /$$$$$$$$  /$$$$$$ | $$  /$$$$$$   /$$$$$$$\n");
    printf("| $$$$$$$/ |____  $$|____ /$$/ /$$__  $$| $$ /$$__  $$ /$$_____/ \n");
    printf("| $$__  $$  /$$$$$$$   /$$$$/ | $$$$$$$$| $$| $$  \\ $$|  $$$$$$  \n");
    printf("| $$  \\ $$ /$$__  $$  /$$__/  | $$_____/| $$| $$  | $$ \\____  $$ \n");
    printf("| $$  | $$|  $$$$$$$ /$$$$$$$$|  $$$$$$$| $$|  $$$$$$/ /$$$$$$$/ \n");
    printf("|__/  |__/ \\_______/|________/ \\_______/|__/ \\______/ |_______/  \n");
    set_attribute(att);
    printf("Copy Right @ Feiyang Wu, Yucheng Zhang, Hanjun Luo and Xuecheng Liu\n");
    printf("Developed Fall 2023, ECE391\n");
    printf("At University of Illinois Urbana Champaign\n");
    printf("Press 'help' for assistance\n");
}
