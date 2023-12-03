#include "calculator.h"
char op[1000], num[1000];
int op_top, num_top;

/*
     *  int cal(char* buf,int32_t length)
     *  DESCRIPTION: calculate the result of the expression
     * INPUTS: buf -- the expression
     *         length -- the length of the expression
     * OUTPUTS: none
     * RETURN VALUE: the result of the expression
     * SIDE EFFECTS: none

*/
int cal(char *buf, int32_t length)
{
    int i;
    char ch;
    memset(op, 0, sizeof(op));
    memset(num, 0, sizeof(num));
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
    return num[0];
}
