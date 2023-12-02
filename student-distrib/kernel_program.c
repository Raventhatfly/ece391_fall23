#include "kernel_program.h"
#include "lib.h"
#include "syscall.h"
#include "page.h"
#include "calculator.h"
#include "serial.h"
int8_t* k_commands[K_COMMAND_NUM];
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
    k_commands[5] = "serial";
    k_handlers[5] = (void*) serial;
    
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
    printf("Alt+F1/F2/F3: change terminal\n");
    printf("Ctrl+C: Halt current process\n");
    printf("Ctrl+L: clear screen\n");
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
            WARNING("you are using the background color.");
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

void serial(int8_t* arg){
    int8_t args[MAX_ARGS][MAX_ARG_LEN + 1];
    int8_t* on = "on";
    int8_t* off = "off";
    int8_t* send = "send";
    // int8_t* recieve = "recieve";
    int8_t* help0 = "-h";
    int8_t* help1 = "help";
    // int8_t* mode = "mode";
    int8_t* loopback = "loopback";
    // int8_t* output = "output";
    int baudrate, i, byte;
    separate_args(arg,args);
    if(strncmp(help0,args[0],2)==0 || strncmp(help1,args[0],4)==0){
        printf("RazelOS Serial Driver Usage:\n");
        printf("[-h/-help]          Get usage\n");
        printf("[on][#number]       Turn on the serial driver with the #number rate\n");
        printf("[off]               Turn off the serial driver\n");
        printf("[send][#byte]       Send the byte from the serial port\n");
        printf("[loopback][0/1]     If the second param is 0 turn off loopback, otherwise loopback\n");
    }else if(strncmp(on,args[0],2)==0){
        baudrate = 0;
        for(i=0;i<MAX_ARG_LEN;i++){
            if(args[1][i] <= '9' && args[1][i] >= '0'){
                baudrate = baudrate * 10 + (int)(args[1][i] - '0');
            }else{
                break;
            }
        }
        if(baudrate == 0){
            printf("Using default rate: 115200 Hz\n");
        }else{
            printf("Using rate:%d\n", baudrate);
        }
        serial_init();
    }else if(strncmp(send,args[0],4)==0){
        byte = 0;
        for(i=0;i<MAX_ARG_LEN;i++){
            if(args[1][i] <= '9' && args[1][i] >= '0'){
                byte = byte * 10 + (int)(args[1][i] - '0');
            }else{
                break;
            }
        }
        serial_send(byte);
    }else if(strncmp(off,args[0],3)==0){
        serial_off();
    }else if(strncmp(loopback,args[0],8)==0){
        if(args[1][0] == '0'){
            serial_loopback_disable();
        }else{
            serial_loopback_enable();
        }
    }else{
        printf("Invalid argument\n");
    }
}

void separate_args(int8_t *arg, int8_t args[MAX_ARGS][MAX_ARG_LEN + 1]){
    int i = 0;
    int j = 0;
    int curr_arg = 0;
    for(i = 0; i<MAX_ARGS;i++){
        for(j=0;j<MAX_ARG_LEN+1;j++){
            args[i][j] = '\0';
        }
    }
    i = 0;
    j = 0;
    /* state 0, waiting for command at the leading spaces;
       state 1, waiting for the next arg
       state 2, reading the current arg
    */
    int state = 0;  
    while(arg[i] != '\0'){
        switch (state)
        {
        case 0:
            if(arg[i] == ' '){
                i++;
            }else{
                state = 2;
            }
            break;
        case 1:
            if(arg[i] != ' '){
                state = 2;
            }else{
                i++;
            }
            break;
        case 2: 
            if(arg[i] == ' '){
                state = 1;
                curr_arg++;
                if(curr_arg >= MAX_ARGS)    return;
                j = 0;
                i++;
            }else if(j >= MAX_ARG_LEN){
                return;
            }else{
                args[curr_arg][j] = arg[i];
                j++; i++;
            }
            break;
        default:
            state = 0;
            break;
        }
        
    }
    if(state == 2 && j != 0){
        curr_arg++;
    }
}
