#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 100

int g_pos; //字符数组的下标

/* 运算符栈 
*  成员内容： 栈底、栈顶、栈容量
*  数据类型： char
*/
typedef struct
{
    char *base;
    char *top;
    int stacksize;
} SqStack;

/* 运算数栈
*  成员内容： 栈底、栈顶、栈容量
*  数据类型： float
*/
typedef struct
{
    float *base;
    float *top;
    int stacksize;
} sqStack;

void InitOPTRStack(SqStack &S);
void InitOPNDStack(sqStack &S);
char GetOPTRTop(SqStack S);
float GetOPNDTop(sqStack S);
void PushOPTR(SqStack &S, char e);
void PushOPND(sqStack &S, float e);
void PopOPTR(SqStack &S, char &e);
void PopOPND(sqStack &S, float &e);
int CheckInput(char e);
float Operate(float a, char theta, float b);
char Precede(char a, char b);
int GetOPNum(char ch);
char Precede(char a, char b);
float Translation(int &pos, char s[]);
float EvaluateExpression(char s[]);

int main(int argc, char *argv[])
{

    int len = strlen(argv[1]);
    argv[1][len] = '#';
    argv[1][len + 1] = '\0';
    printf("%g", EvaluateExpression(argv[1]));

    return 0;
}

/* 栈初始化
*  参数：栈地址，若成功则返回栈头地址，不成功则 OVERFLOW
*  OPTR
*/
void InitOPTRStack(SqStack &S)
{
    S.base = (char *)malloc(STACK_INIT_SIZE * sizeof(char));
    if (!S.base)
        exit(OVERFLOW);
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
}

/* 栈初始化
*  参数：栈地址，若成功则返回栈头地址，不成功则 OVERFLOW
*  OPND
*/
void InitOPNDStack(sqStack &S)
{
    S.base = (float *)malloc(STACK_INIT_SIZE * sizeof(float));
    if (!S.base)
        exit(OVERFLOW);
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
}

/* 返回栈顶元素 */
char GetOPTRTop(SqStack S)
{
    if (S.top == S.base)
        printf("ERROR_01");
    return *(S.top - 1);
}

/* 返回栈顶元素 */
float GetOPNDTop(sqStack S)
{
    if (S.top == S.base)
        exit(1);
    return *(S.top - 1);
}

/*  OPTR入栈
*   若栈满则realloc 
*/
void PushOPTR(SqStack &S, char e)
{
    if (S.top - S.base >= S.stacksize)
    {
        S.base = (char *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(char));
        if (!S.base)
            exit(OVERFLOW);
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
    }
    *S.top++ = e;
}

/*  OPND入栈
*   若栈满则realloc 
*/
void PushOPND(sqStack &S, float e)
{
    if (S.top - S.base >= S.stacksize)
    {
        S.base = (float *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(float));
        if (!S.base)
            exit(OVERFLOW);
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
    }
    *S.top++ = e;
}

/*  OPTR出栈
*   若栈空则报错（本题具体要求）
*/
void PopOPTR(SqStack &S, char &e)
{
    if (S.top == S.base)
    {
        printf("ERROR_02");
        exit(1);
    }
    e = *--S.top;
}

/*  OPND出栈
*   若栈空则报错（本题具体要求）
*/
void PopOPND(sqStack &S, float &e)
{
    if (S.top == S.base)
    {
        printf("ERROR_02");
        exit(1);
    }
    e = *--S.top;
}

/* 判断读入字符 */
int CheckInput(char e)
{
    int a;
    if (e == '+' || e == '-' || e == '*' || e == '/' || e == '(' || e == ')' || e == '^' ||
        e == '[' || e == ']' || e == '#')
        a = 1; //是运算符
    else if (isdigit(e) || e == '.')
        a = 2; //是数字或者小数点
    else
    {
        printf("ERROR_02");
        exit(3);
    }
    return a;
}

/* 运算
*  输入参数 float a, char theta, float b
*  返回 float result
*/
float Operate(float a, char theta, float b)
{
    float c;

    if (theta == '+')
        c = a + b;
    else if (theta == '-')
        c = a - b;
    else if (theta == '*')
        c = a * b;
    else if (theta == '/')
        c = a / b;
    else if (theta == '^')
        c = pow(a, b);
    else
        printf("ERROR_02"); // exit(1);
    return c;
}

/* 获取运算符编号 1~9
*  若输入错误返回 -1
*/
int GetOPNum(char ch)
{
    char op[] = {'+', '-', '*', '/', '(', ')', '[', ']', '^', '#'};
    for (int i = 0; i < 10; i++)
    {
        if (op[i] == ch)
        {
            return i;
        }
    }
    return -1;
}

/* 比较运算符的优先级 */
char Precede(char a, char b)
{
    int i, j;
    char f;
    char Table[10][10] = {
        /*    +     -     *     /     (     )     [     ]     ^     #      */
        /* + */ {'>', '>', '<', '<', '<', '>', '<', '>', '<', '>'},
        /* - */ {'>', '>', '<', '<', '<', '>', '<', '>', '<', '>'},
        /* * */ {'>', '>', '>', '>', '<', '>', '<', '>', '<', '>'},
        /* / */ {'>', '>', '>', '>', '<', '>', '<', '>', '<', '>'},
        /* ( */ {'<', '<', '<', '<', '<', '=', '?', '?', '<', '?'},
        /* ) */ {'>', '>', '>', '>', '?', '>', '?', '?', '>', '>'},
        /* [ */ {'<', '<', '<', '<', '<', '?', '<', '=', '<', '?'},
        /* ] */ {'>', '>', '>', '>', '?', '?', '?', '>', '>', '>'},
        /* ^ */ {'>', '>', '>', '>', '<', '>', '<', '>', '>', '>'},
        /* # */ {'<', '<', '<', '<', '<', '?', '<', '?', '<', '='},
    };
    f = Table[GetOPNum(a)][GetOPNum(b)];
    if (f == '?')
    {
        printf("ERROR_02");
        exit(1);
    }
    else
        return f;
}

/* char-float转换，取完整数字 */
float Translation(int &pos, char s[])
{
    float integer = 0.0;   // 整数部分
    float remainder = 0.0; // 小数部分

    while (s[pos] >= '0' && s[pos] <= '9')
    {
        integer *= 10;
        integer += (s[pos] - '0');
        pos++;
    }

    if (s[pos] == '.')
    {
        pos++;
        int c = 1;
        while (s[pos] >= '0' && s[pos] <= '9')
        {
            float t = s[pos] - '0';
            t *= pow(0.1, c);
            c++;
            remainder += t;
            pos++;
        }
    }

    return integer + remainder;
}

/* 通过栈进行计算 */
float EvaluateExpression(char s[])
{
    SqStack(OPTR);
    sqStack(OPND);
    InitOPNDStack(OPND);
    InitOPTRStack(OPTR);

    char c, x, theta;
    float a, b;
    int len = strlen(s);

    PushOPTR(OPTR, '#');
    for (g_pos = 0; g_pos < len;)
    {
        c = s[g_pos];
        if (CheckInput(c) == 2)
        {
            PushOPND(OPND, Translation(g_pos, s));
        }
        else if (CheckInput(c) == 1)
        {
            switch (Precede(GetOPTRTop(OPTR), s[g_pos]))
            {
            case '<':
                PushOPTR(OPTR, c);
                g_pos++;
                break;
            case '>':
                PopOPTR(OPTR, theta);
                PopOPND(OPND, b);
                PopOPND(OPND, a);
                if (theta == '/' && b == 0.0)
                {
                    printf("ERROR_03");
                    exit(1);
                }
                else
                    PushOPND(OPND, Operate(a, theta, b));
                break;
            case '=':
                PopOPTR(OPTR, x);

                g_pos++;
                break;
            }
        }
    }
    return GetOPNDTop(OPND);
}