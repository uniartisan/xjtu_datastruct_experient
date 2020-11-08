#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1 //表达式不合法
#define ERROR_02 1e9 + 2
#define ERROR_03 -1e9
#define STACK_MAX_LEN 1000 //字符串的长度最大值
#define InitValue 0.0      //用于初始化变量
#define InitValue2 1.0     //初始化变量
#define ERROR_NoNext -1    //定义空栈错误返回值

// 定义堆栈1用于存放数字

typedef struct SNode1 *StackOfNumbers;
struct SNode1
{
    double Operand;
    StackOfNumbers Next;
};

// 定义堆栈2用于存放运算符

typedef struct SNode2 *StackOfOperator;
struct SNode2
{
    char Operator;
    StackOfOperator Next;
    //int StackSize;
};

char op[] = {'+', '-', '*', '/', '^', '(', ')', '[', ']'}; // 运算符集合
char pre[9][9] = {
    {'>', '>', '<', '<', '<', '<', '>', '<', '>'},
    {'>', '>', '<', '<', '<', '<', '>', '<', '>'},
    {'>', '>', '>', '>', '<', '<', '>', '<', '>'},
    {'>', '>', '>', '>', '<', '<', '>', '<', '>'},
    {'>', '>', '>', '>', '>', '<', '>', '<', '>'},
    {'<', '<', '<', '<', '<', '<', '=', '0', '0'},
    {'>', '>', '>', '>', '>', '0', '>', '0', '0'},
    {'<', '<', '<', '<', '<', '<', '0', '<', '='},
    {'>', '>', '>', '>', '>', '0', '0', '0', '>'}};
StackOfNumbers InitStackofNumbers(); //初始化堆栈1
StackOfOperator InitStackofOper();   //初始化堆栈2

void PushtoNumbers(StackOfNumbers S, double data); //数据data存入堆栈1
double Pop1(StackOfNumbers S);

void PushtoOper(StackOfOperator S, char oper);
char Pop2(StackOfOperator S);
char GetTop(StackOfOperator S);
int GetOPLocation(char ch);
int CheckinOp(char c);
int CheckinOp_s(char c);
int CheckInputErrors(int input_num, char express[]);
int CheckBracketsErrors(char express[]);
char Precede(char s1, char s2);
double Caculate(double a1, double a2, char s);
double EvaluateExpression(char Expression[]);
int InputCheck(int input_num, char *expression_raw, int expression_raw_lenth);
int NumStackLenth = 1;
int OpeStackLenth = 1;
// List Transfer(char *Expression);                //把输入的字符型表达式转化为数字和运算符的表达式

int main(int argc, char *argv[])
{
    float OPND[1001] = {0};
    int OPND_TOP = 0;
    int input_num;
    char expression[STACK_MAX_LEN];
    double Number[101] = {};

    input_num = 2;
    scanf("%s", expression);
    int input_lenth; //输入字符串长度
    input_lenth = strlen(expression);

    // ERROR_01:输入参数错误
    // ERROR_02:表达式格式错误
    // ERROR_03:表达式逻辑错误
    int input_check_result = CheckInputErrors(input_num, expression);
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

    double value = EvaluateExpression(expression); //计算表达式的值，并检查计算过程中是否合法
    if (value == ERROR_03)                         //表达式计算过程中出现错误
    {
        printf("ERROR_03");
    }
    else
    {
        printf("%g", value); //输出正确的值
    }
    return 0;
}

//初始化堆栈1，带有头结点
StackOfNumbers InitStackofNumbers()
{
    StackOfNumbers S;
    S = (StackOfNumbers)malloc(sizeof(struct SNode1));
    S->Next = NULL;
    return S;
}

//初始化堆栈2，带有头结点
StackOfOperator InitStackofOper()
{
    StackOfOperator S;
    S = (StackOfOperator)malloc(sizeof(struct SNode2));
    S->Next = NULL;
    return S;
}

/* 移除数据堆栈栈顶的元素
*  如果空栈则返回 ERROR_NoNext
*  程序内部已经完成内存释放
*/
double Pop1(StackOfNumbers S)
{
    if (!S->Next)
        return ERROR_NoNext;
    double TemData;
    StackOfNumbers Tem;
    NumStackLenth--;
    Tem = S->Next;
    TemData = Tem->Operand;
    S->Next = Tem->Next;
    free(Tem); //释放使用的内存
    return TemData;
}

/* 移除运算符堆栈栈顶的元素
*  如果空栈则返回 NULL
*  程序内部已经完成内存释放
*/
char Pop2(StackOfOperator S)
{
    char error[] = "ERRORNoNext";
    // if (!S->Next)
    //     return NULL;
    char TemOpera;
    StackOfOperator Tem;
    OpeStackLenth--;
    Tem = S->Next;
    TemOpera = Tem->Operator;
    S->Next = Tem->Next;
    free(Tem); //释放使用的内存
    return TemOpera;
}

char GetTop(StackOfOperator S)
{
    if (NULL == S->Next)
    {
        return '\000';
    }
    // StackOfOperator Tem;
    // Tem = S->Next;
    // while (NULL!=Tem)
    // {
    //     Tem = S->Next;
    // }

    return (S->Next->Operator);
}

/* 将双精度数据data存入数字堆栈
*  程序内部通过malloc申请节点内存空间
*/
void PushtoNumbers(StackOfNumbers S, double data)
{
    StackOfNumbers Tem;
    NumStackLenth++;
    Tem = (StackOfNumbers)malloc(sizeof(struct SNode1));
    Tem->Operand = data;
    Tem->Next = S->Next;
    S->Next = Tem;
}

/* 将字符数据数据oper存入运算符堆栈
*  程序内部通过malloc申请节点内存空间
*/
void PushtoOper(StackOfOperator S, char oper)
{
    StackOfOperator Tem;
    OpeStackLenth++;
    Tem = (StackOfOperator)malloc(sizeof(struct SNode2));
    Tem->Operator = oper;
    Tem->Next = S->Next;
    S->Next = Tem;
}

/* 检查原始表达式错误
*  匹配返回值：1，代表无错误
*  错误返回值： -2，代表输入错误； -1，代表参数错误
*/
int CheckInputErrors(int input_num, char express[])
{
    if (input_num != 1 + 1) //输入一个有效参数
    {
        return -1;
    }
    int expression_raw_lenth = strlen(express); //统计字符串长度
    //表达式格式校验
    for (int i = 0; i < expression_raw_lenth; i++)
    {
        //校验是否存在字符非法
        if (express[i] >= '0' && express[i] <= '9' || express[i] == '.')
        {
            continue;
        }
        else //教研运算逻辑输入非法
        {
            if (!CheckinOp(express[i]) && i != expression_raw_lenth - 1)
            {
                return -2;
            }
            else if ((express[i + 1] < '0' || express[i + 1] > '9') && (!CheckinOp(express[i])))
            //如果在输入的最后一个为运算符，那么由于有\n所以满足了前两个判断
            {
                return -2;
            }
            if (CheckinOp_s(express[0]))
            {
                return -2;
            }
        }
    }
    return (CheckBracketsErrors(express));
}

/* 检查原始表达式中括号是否匹配
*  匹配返回值：1，代表无错误
*  错误返回值： -2，代表括号匹配存在问题
*  通过模拟栈的匹配完成，减轻代码难度提升可读性
*/
int CheckBracketsErrors(char express[])
{
    int lenth = strlen(express); //统计字符串长度
    int temp0 = 0;
    int temp1 = 0;
    int t = 0, Bracket_small = 0, Bracket_big = 0;
    for (int i = 0; i < lenth; i++) //提取所有括号
    {

        if (express[i] == '(')
        {
            temp0++;
            if (Bracket_small < i)
                Bracket_small = i;
        }
        if (express[i] == ')')
        {
            temp0--;
        }
        if (express[i] == '[')
        {
            temp1++;
            if (Bracket_small < i)
                Bracket_big = i;
        }
        if (express[i] == ']')
        {
            temp1--;
        }
        // }
    }
    /* 检测包括：
    *  1. 括号匹配  2. 小括号在大括号内
    */
    if (Bracket_small < Bracket_big)
    {
        return -2;
    }

    if (temp0 || temp1)
    {
        return -2;
    }
    return 1;
}

// 检查运算符，若存在返回1
int CheckinOp(char c)
{
    int lenth = strlen(op);
    int i = 0;
    for (i = 0; i < lenth; i++)
    {
        if (c == op[i])
        {
            return 1;
        }
    }
    return 0;
}

// 严格检查运算符： +，-，*，/，^ ,若存在返回1
int CheckinOp_s(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double EvaluateExpression(char Expression[])
{
    double CheckError3 = 1.1;                   //初始化为非0值
    StackOfNumbers OPND = InitStackofNumbers(); //初始化堆栈1
    StackOfOperator OPTR = InitStackofOper();   //初始化堆栈2

    int i = 0, j = 0;
    int lenth = strlen(Expression);
    char NumTemp[STACK_MAX_LEN] = {};
    char TemOp;
    char TemOp2;
    double TemData = 0.1, TemData2 = 0.1; //初始化临时变量
    int flag = 0;
    //StackOfOperator = StackOfOperator->Next;
    while (i <= lenth)
    {
        if (Expression[i] >= '0' && Expression[i] <= '9' && i < lenth)
        {
            //flag = 1;
            while (Expression[i] >= '0' && Expression[i] <= '9' || Expression[i] == '.')
            {
                NumTemp[j++] = Expression[i++];
            }
            if (i <= lenth)
                flag = -1;
            NumTemp[j] = '\n';
        }
        else
        {
            TemOp = Expression[i]; //当前运算符
            TemOp2 = GetTop(OPTR); //栈顶运算符
            char Presult;
            Presult = Precede(TemOp2, TemOp);
            if (i <= lenth)
            {
                j = 0;
                if (flag == -1 && (Expression[i] < '0' || Expression[i] > '9'))
                {
                    double temp0 = atof(NumTemp);
                    NumTemp[0] = '\n';
                    flag = 1;
                    PushtoNumbers(OPND, temp0);
                }
                if (i == lenth)
                    flag = 1;
            }

            switch (Presult)
            {
            case '<':
                PushtoOper(OPTR, TemOp);
                i++;
                break;
            case '=':
                Pop2(OPTR);
                i++;
                break;
            case '>':

                Pop2(OPTR);
                TemData2 = Pop1(OPND);
                TemData = Pop1(OPND);
                while ((GetTop(OPTR) == '(' || GetTop(OPTR) == ')' || GetTop(OPTR) == '[' || TemOp2 == ']') && OpeStackLenth > 1)
                {
                    Pop2(OPTR);
                }
                // if (CheckinOp(Expression[i + 1]))
                // {
                //     break;
                // }
                i++;
                CheckError3 = Caculate(TemData, TemData2, TemOp2);
                if (TemOp != '\000')
                    PushtoOper(OPTR, TemOp); //当前运算符入栈
                if (CheckError3 == ERROR_03)
                    return ERROR_03; //存在错误情况ERROR_03
                else
                {
                    PushtoNumbers(OPND, CheckError3);
                }

                // default:
                //     i++;
                //     break;
            }
        }
    }
    while (NumStackLenth > 2)
    {
        /* code */
        TemData2 = Pop1(OPND);
        TemData = Pop1(OPND);
        TemOp2 = Pop2(OPTR);
        while (TemOp2 == '(' || TemOp2 == ')' || TemOp2 == '[' || TemOp2 == ']')
        {
            TemOp2 = Pop2(OPTR);
        }
        CheckError3 = Caculate(TemData, TemData2, TemOp2);

        //PushtoOper(OPTR, TemOp);
        if (CheckError3 == ERROR_03)
            return ERROR_03; //存在错误情况ERROR_03
        else
        {
            PushtoNumbers(OPND, CheckError3);
            //i++;
        }
    }

    return Pop1(OPND);
}

char Precede(char s1, char s2)
{
    if (s1 == '\000')
    {
        return '<';
    }
    if (s2 == '\000')
    {
        return '>';
    }
    int t1 = GetOPLocation(s1);
    int t2 = GetOPLocation(s2);
    return pre[t1][t2];
}

double Caculate(double a1, double a2, char s)
{
    if (s == '+')
        return a1 + a2;
    if (s == '-')
        return a1 - a2;
    if (s == '*')
        return a1 * a2;
    if (s == '/')
    {
        double temp = 0.00001;
        if (a2 != 0)
            return a1 / a2;
        else
            return -1e9;
    }
    if (s == '^')
    {
        double temp2 = 1;
        for (int i = 1; i <= a2; i++)
        {
            temp2 *= a1;
        }
        return temp2;
    }
}

// 找到运算符的位置，未找到返回-1
int GetOPLocation(char ch)
{
    for (int i = 0; i < 9; i++)
    {
        if (op[i] == ch)
        {
            return i;
        }
    }
    return -1;
}
