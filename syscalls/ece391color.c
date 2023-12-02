#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024

void show_help();
int main(){
    uint8_t buf[BUFSIZE];
    int i,num;
    uint8_t* help = "help";
    uint8_t* h = "-h";
    if (0 != ece391_getargs (buf, BUFSIZE)) {
        ece391_fdputs (1, (uint8_t*)"could not read argument\n");
	return 3;
    }

    for(i=0;i<BUFSIZE;i++){
        if(buf[i]==' ' || buf[i] == '\0')   break;
    }

    if('0'<=buf[0] && buf[0]<= '9'){
        if(buf[0] == '0'){
            ece391_fdputs(1, "You are using the background color.\n");
        }
        num = (int)(buf[0] - '0');
    }else if('a'<= buf[0] && buf[0]<='f'){
        num = (int)(buf[0] - 'a' + 10);
    }else if(ece391_strncmp(buf,help,4) == 0){
        show_help();
        return 0;
    }else if(ece391_strncmp(buf,h,2) == 0){
        show_help();
        return 0;
    }else{
        ece391_fdputs(1,"Please input argument as a single hedeximal number\n");
        ece391_fdputs(1,"Type 'color help' for assistance.\n");
        return 2;
    }
    ece391_set_color(num);
    return 0;
}

void show_help(){
    ece391_fdputs(1, " Color Table\n");
    ece391_fdputs(1, " ------------------------\n");
    ece391_fdputs(1, "| code  |     color     |\n");
    ece391_fdputs(1, "|   0   |     black     |\n");
    ece391_fdputs(1, "|   1   |     blue      |\n");
    ece391_fdputs(1, "|   2   |     green     |\n");
    ece391_fdputs(1, "|   3   |     cyan      |\n");
    ece391_fdputs(1, "|   4   |      red      |\n");
    ece391_fdputs(1, "|   5   |    magenta    |\n");
    ece391_fdputs(1, "|   6   |     brown     |\n");
    ece391_fdputs(1, "|   7   |   light gray  |\n");
    ece391_fdputs(1, "|   8   |   dark gray   |\n");
    ece391_fdputs(1, "|   9   |   light blue  |\n");
    ece391_fdputs(1, "|   a   |  light green  |\n");
    ece391_fdputs(1, "|   b   |   light cyan  |\n");
    ece391_fdputs(1, "|   c   |   light red   |\n");
    ece391_fdputs(1, "|   d   | light magenta |\n");
    ece391_fdputs(1, "|   e   |     yellow    |\n");
    ece391_fdputs(1, "|   f   |     white     |\n");
    ece391_fdputs(1, " ------------------------\n");
}