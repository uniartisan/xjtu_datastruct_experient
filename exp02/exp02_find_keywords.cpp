#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define NotFound -1
#define MallocError -1

void Next(char *T, int *next);
int KMP(char *S, char *T, int *next);

int main(int argc, char *argv[])
{
    /* Check Input Num_ERROR */
    if (argc != 3)
    {
        printf("ERROR_01");
        return -1;
    }

    /* Only for debug */
    // printf("string: %s\n", argv[1]);
    // printf("keywords: %s\n", argv[2]);

    int StrLen = 0, KeyLen = 0;
    StrLen = strlen(argv[1]);
    KeyLen = strlen(argv[2]);
    // printf("%d %d\n",StrLen,KeyLen);
    int *NextNum;
    char *string, *keywords;
    string = (char *)malloc(StrLen * sizeof(char));
    keywords = (char *)malloc(KeyLen * sizeof(char));
    NextNum = (int *)malloc((KeyLen + 1) * sizeof(int));
    /* Check malloc */
    if (string == NULL || keywords == NULL || NextNum == NULL)
    {
        return MallocError;
    }

    for (int i = 0; i < KeyLen + 1; i++)
    {
        NextNum[i] = 0;
    }
    strcpy(string, argv[1]);
    strcpy(keywords, argv[2]);
    Next(keywords, NextNum); //根据模式串keywords,初始化next数组

    int result = NotFound;
    result = KMP(string, keywords, NextNum);
    printf("%d", result);
    free(string);
    free(keywords);
    free(NextNum);
    return 0;
}

/* 找到模式串的Next数组
*  输入：char[] 模式串、int[] Next数组
*/
void Next(char *T, int *next)
{
    next[1] = 0;
    next[2] = 1;
    int i = 2;
    int j = 1;
    while (i < strlen(T))
    {
        if (j == 0 || T[i - 1] == T[j - 1])
        {
            i++;
            j++;
            if (T[i - 1] != T[j - 1])
            {
                next[i] = j;
            }
            else
            {
                next[i] = next[j];
            }
        }
        else
        {
            j = next[j];
        }
    }
}

int KMP(char *S, char *T, int *next)
{

    int i = 1;
    int j = 1;
    while (i <= strlen(S) && j <= strlen(T))
    {
        //j==0:代表模式串的第一个字符就和当前测试的字符不相等；S[i-1]==T[j-1],如果对应位置字符相等，两种情况下，指向当前测试的两个指针下标i和j都向后移
        if (j == 0 || S[i - 1] == T[j - 1])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j]; //如果测试的两个字符不相等，i不动，j变为当前测试字符串的next值
        }
    }
    if (j > strlen(T))
    { //如果条件为真，说明匹配成功
        return i - (int)strlen(T);
    }
    //匹配失败
    else
    {
        return NotFound;
    }
}