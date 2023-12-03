#include <stdint.h>
#include <stdio.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024

int main()
{
    char buf[BUFSIZE];
    char output_buf[50];
    char op[BUFSIZE], num[BUFSIZE];
    int op_top, num_top;
    int i, result, val;
    int minus_flag;
    char ch;
    int32_t length;
    
    if (0 != ece391_getargs (buf, BUFSIZE)) {
        ece391_fdputs (1, (uint8_t*)"could not read argument\n");
	    return 3;
    }
    length = ece391_strlen((uint8_t*)buf);

    for(i=0;i<BUFSIZE;i++){
        op[i] = 0;
        num[i] = 0;
    }
    for(i=0;i<50;i++){
        output_buf[i] = 0;
    }
    num_top =0;
    op_top = -1;
    for (i = 0; i < length; i++)
    {
        if (buf[i] == '\n') break;
        ch = buf[i];
        if (buf[i]>='0' && buf[i]<='9') num[num_top]=num[num_top]*10+ch-'0';
        else 
        {
            if (ch == '(') op[++op_top] = ch;
            else if (ch == ')')
            {
                while (op[op_top] != '(')
                {
                    if (op[op_top] == '+') num[num_top - 1] += num[num_top];
                    else if (op[op_top] == '-') num[num_top - 1] -= num[num_top];
                    else if (op[op_top] == '*') num[num_top - 1] *= num[num_top];
                    else if (op[op_top] == '/') num[num_top - 1] /= num[num_top];
                    num[num_top]=0;
                    op_top--;
                    num_top--;
                }
                op_top--;
            }
            else if (ch == '+' || ch == '-')
            {
                while (op_top >= 0 && op[op_top] != '(')
                {
                    if (op[op_top] == '+') num[num_top - 1] += num[num_top];
                    else if (op[op_top] == '-') num[num_top - 1] -= num[num_top];
                    else if (op[op_top] == '*') num[num_top - 1] *= num[num_top];
                    else if (op[op_top] == '/') num[num_top - 1] /= num[num_top];
                    num[num_top]=0;
                    op_top--;
                    num_top--;
                }
                op[++op_top] = ch;
                num_top++;
            }
            else if (ch == '*' || ch == '/')
            {
                while (op_top >= 0 && op[op_top] != '(' && op[op_top] != '+' && op[op_top] != '-')
                {
                    if (op[op_top] == '*') num[num_top - 1] *= num[num_top];
                    else if (op[op_top] == '/') num[num_top - 1] /= num[num_top];
                    num[num_top]=0;
                    op_top--;
                    num_top--;
                }
                op[++op_top] = ch;
                num_top++;
            }
        }
    }
    while (op_top >= 0)
    {
        if (op[op_top] == '+') num[num_top - 1] += num[num_top];
        else if (op[op_top] == '-') num[num_top - 1] -= num[num_top];
        else if (op[op_top] == '*') num[num_top - 1] *= num[num_top];
        else if (op[op_top] == '/') num[num_top - 1] /= num[num_top];
        op_top--;
        num_top--;
    }
    result = num[0];
    minus_flag = 0;
    if(result < 0){
        result = -result;
        minus_flag = 1;
    }
    i = 0;
    while(result > 0){
        val = result - (result / 10) * 10;
        output_buf[i] = (char)val + '0';
        result = result / 10;
        i++;
    }
    output_buf[i] = '\n';
    output_buf[i+1] = '\0';
    ece391_fdputs(1,"The result is:");
    if(minus_flag){
        ece391_fdputs(1,"-");
    }
    if(num[0] == 0){
        ece391_fdputs(1,"0\n");
    }else{
        ece391_fdputs(1, output_buf);
    }
    
    return 0;
}
