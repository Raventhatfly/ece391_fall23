#include <stdint.h>
#include <stdio.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024

char op[1000], num[1000];
int op_top, num_top;

int main()
{
    char buf[BUFSIZE];
    char output_buf[50];
    int i, result, val;
    char ch;
    int32_t length;
    
    if (0 != ece391_getargs (buf, BUFSIZE)) {
        ece391_fdputs (1, (uint8_t*)"could not read argument\n");
	    return 3;
    }
    length = ece391_strlen((uint8_t*)buf);

    for(i=0;i<1000;i++){
        op[i] = 0;
        num[i] = 0;
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
    i = 0;
    while(result > 0){
        val = result - (result / 10) * 10;
        output_buf[i] = (char)val + '0';
        result = result / 10;
        i++;
    }
    output_buf[i] = '\n';
    ece391_fdputs(1,"The result is:");
    ece391_fdputs(1, output_buf);
    return 0;
}
