#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"
#define BUFSIZE 1024
int main(){
    ece391_fdputs(1, "Welcome to RazelOS!\n");
    ece391_fdputs(1, " /$$$$$$$                                /$$                    \n");
    ece391_fdputs(1, "| $$__  $$                              | $$                    \n");
    ece391_fdputs(1, "| $$  \\ $$  /$$$$$$  /$$$$$$$$  /$$$$$$ | $$  /$$$$$$   /$$$$$$$\n");
    ece391_fdputs(1, "| $$$$$$$/ |____  $$|____ /$$/ /$$__  $$| $$ /$$__  $$ /$$_____/ \n");
    ece391_fdputs(1, "| $$__  $$  /$$$$$$$   /$$$$/ | $$$$$$$$| $$| $$  \\ $$|  $$$$$$  \n");
    ece391_fdputs(1, "| $$  \\ $$ /$$__  $$  /$$__/  | $$_____/| $$| $$  | $$ \\____  $$ \n");
    ece391_fdputs(1, "| $$  | $$|  $$$$$$$ /$$$$$$$$|  $$$$$$$| $$|  $$$$$$/ /$$$$$$$/ \n");
    ece391_fdputs(1, "|__/  |__/ \\_______/|________/ \\_______/|__/ \\______/ |_______/  \n");
    ece391_fdputs(1, "Copy Right @ Feiyang Wu, Yucheng Zhang, Hanjun Luo and Xuecheng Liu\n");
    ece391_fdputs(1, "Developed Fall 2023, ECE391\n");
    ece391_fdputs(1, "At University of Illinois Urbana Champaign\n");
    ece391_fdputs(1, "Alt+F1/F2/F3: change terminal\n");
    ece391_fdputs(1, "Ctrl+C: Halt current process\n");
    ece391_fdputs(1, "Ctrl+L: clear screen\n");
    return 0;
}