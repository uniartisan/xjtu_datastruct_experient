#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_MAX_LEN 10000
#define SElemType float
#define Status int
#define overflow -2
#define error -1
#define success 1

typedef struct
{
    SElemType *base; //栈底指针，构造前和销毁后为NULL
    SElemType *top;  //栈顶指针
    int stacksize;
} seqstack;
Status init_stack(seqstack *seq);
Status destroy_stack(seqstack *seq);
Status get_top(seqstack seq, SElemType *e);
int get_lenth(seqstack seq);
Status push(seqstack *seq, SElemType e);
Status pop(seqstack *seq, SElemType *e);

char op[] = {'+', '-', '*', '/', '^', '(', ')', '[', ']'};
int check_in_op(char c);
int input_check(int input_num, char *expression_raw, int expression_raw_lenth); //输入合法性校验

int main(int argc, char *argv[])
{

    int input_num;
    char expression[STACK_MAX_LEN];

    input_num = 2;
    scanf("%s", expression);
    int input_lenth; //输入参数长度
    input_lenth = strlen(expression);

    // ERROR_01:输入参数错误
    // ERROR_02:表达式格式错误
    // ERROR_03:表达式逻辑错误
    int input_check_result = input_check(input_num, expression, input_lenth);
    if (input_check_result == -1)
    {
        printf("ERROR_01\n");
        return -1;
    }
    else if (input_check_result == -2)
    {
        printf("ERROR_02\n");
        return -1;
    }
    else if (input_check_result == -3)
    {
        printf("ERROR_03\n");
        return -1;
    }

    return 0;
}

int input_check(int input_num, char *expression_raw, int expression_raw_lenth)
{
    // return -1:输入参数错误
    // return -2:表达式格式错误
    // return -3:表达式逻辑错误
    // return 1: 校验合法
    char operator[STACK_MAX_LEN]; //运算符，用于格式校验
    int operator_lenth = 0, i = 0;
    //输入参数校验
    if (input_num != 1 + 1) //输入一个有效参数
    {
        return -1;
    }
    //表达式格式校验
    for (i = 0; i < expression_raw_lenth; i++)
    {
        if (expression_raw[i] >= '0' && expression_raw[i] <= '9' || expression_raw[i] == '.')
        {
            continue;
        }
        else
        {
            if (!check_in_op(expression_raw[i]))
            {
                return -2;
            }
            else if ((expression_raw[i + 1] < '0' || expression_raw[i + 1] > '9') && i != expression_raw_lenth - 1) //如果在输入的最后一个为运算符，那么由于有\n所以满足了前两个判断
            {
                return -2;
            }
            else if (i == 0)
            {
                if (expression_raw[i] == '+' || expression_raw[i] == '-' || expression_raw[i] == '*' || expression_raw[i] == '/' || expression_raw[i] == '^')
                {
                    return -2;
                }
            }
            if (expression_raw[i] == '(' || expression_raw[i] == ')' || expression_raw[i] == '[' || expression_raw[i] == ']')
            {
                operator[operator_lenth] = expression_raw[i];
                operator_lenth++;
            }

            // 校验表达式合法性,计算过程中需要再次检测
            if (expression_raw[i] == '/' && expression_raw[i + 1] == '0')
            {
                if (i + 1 < expression_raw_lenth && expression_raw[i + 2] == '.')
                    continue;
                else
                {
                    return -3;
                }
            }
        }
    }

    // 1.右括号在左括号前面
    // 2.大括号在小括号里面
    // 3.括号不匹配
    if (operator_lenth % 2 != 0)
    {
        return -2;
    }
    int a = 0, b = 0; //记录大括号小括号位置
    for (i = 0; i < operator_lenth / 2; i++)
    {
        if (operator[i] == '(')
        {
            if (operator[operator_lenth - 1 - i] != ')')
            {
                return -2;
            }
            if (b == 0)
            {
                b = i;
            }
        }
        if (operator[i] == '[')
        {
            if (operator[operator_lenth - 1 - i] != ']')
            {
                return -2;
            }
            if (a == 0)
            {
                a = i;
            }
        }
    }
    if (a > b)
    {
        return -2;
    }
}

int check_in_op(char c)
{
    int lenth = strlen(op);
    int i = 0;
    for (i = 0; i < lenth; i++)
    {
        if (c == op[i])
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
// Stack 实现
Status init_stack(seqstack *seq)
{
    //构造一个栈
    seq->base = (SElemType *)malloc(STACK_MAX_LEN * sizeof(SElemType));
    if (!seq->base)
    {
        return overflow;
    }
    seq->top = seq->base;
    seq->stacksize = STACK_MAX_LEN;
    return success;
}

Status destroy_stack(seqstack *seq)
{
    //销毁栈,如果栈不存在返回错误
    if (seq->base != NULL)
    {
        for (int i = 0; i < STACK_MAX_LEN; i++)
        {
            free(seq->base);
            seq->base++;
        }
        seq->base = seq->top = NULL;
        seq->stacksize = 0;
        return success;
    }
    else
    {
        return error;
    }
}

Status get_top(seqstack seq, SElemType *e)
{
    //若栈不空，返回顶部值，否则返回错误
    if (seq.top == seq.top)
    {
        return error;
    }
    *e = *(seq.top - 1);
}

int get_lenth(seqstack seq)
{
    return seq.top - seq.base;
}

Status push(seqstack *seq, SElemType e)
{
    //如果栈满报错
    if (seq->top - seq->base >= STACK_MAX_LEN)
    {
        return error;
    }
    *seq->top++ = e;
    return success;
}

Status pop(seqstack *seq, SElemType *e)
{
    //如果栈不空，则删除元素，并用e返回值，否则报错
    if (seq->top == seq->base)
    {
        return error;
    }
    *e = *--seq->top;
    return success;
}