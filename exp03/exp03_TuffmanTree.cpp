#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define OK 1
#define FALSE 0
#define STACKINCREMENT 20
typedef struct
{
	char data;
	unsigned int weight;   //权值
	unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree;   //动态分配数组存储赫夫曼树
typedef char** HuffmanCode;//动态分配数组存储赫夫曼编码表

typedef struct
{
	int *base;
	int *top;
	int stacksize;
}SqStack;

int InitStack(SqStack &S,int n)
{
	S.base = (int*)malloc((2 * n - 1) * sizeof(int));//该树总共有2*n-1个节点，开辟2*n-1个空间
	if (!S.base)
	{
		return FALSE;
	}
	S.top = S.base;
	S.stacksize = 2 * n - 1;
	return OK;
}

int Pop(SqStack &S, int &temp)
{
	if (S.top == S.base)
	{
		return FALSE;
	}
	S.top=S.top-1;
	temp = *(S.top);
	return OK;
}

int Push(SqStack &S, int temp)
{
	if ((S.top - S.base) >= S.stacksize)
	{
		S.base = (int *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(int));
		if (!S.base)
		{
			return FALSE;
		}
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*(S.top) = temp;
	S.top=S.top +1;
	return OK;
}

int StackEmpty(SqStack S)
{
	if (S.base == S.top)
	{
		return OK;
	}
		return FALSE;
}

//HT;输出，构造的赫夫曼树
//HC：输出，n个不同字符的赫夫曼编码
//w:输入，存放n个不同字符的权值
//v：输入，存放n个不同字符的数组
//n:输入，命令行参数输入不同字符的个数
int HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, unsigned int* w, char *v, int n)
{
	int Select(HuffmanTree HT, int n);
	if (n <= 1)
	{
		return FALSE;
	}
	int m = 2 * n - 1;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));//0号单元未用
	if (!HT)
	{
		return FALSE;
	}
	int i = 1;
	for (i; i <= n; ++i, ++w, ++v)
	{
		*(HT + i) = { *v,*w,0,0,0 };
	}
	for (; i <= m; ++i)
	{
		*(HT + i) = { 0,0,0,0,0 };
	}
	for (int i = n + 1; i <= m; ++i)//建造赫夫曼树
	{
		int s1 = 0, s2 = 0;
		s1 = Select(HT, i - 1);
		s2 = Select(HT, i - 1);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));//分配n个字符编码的头指针向量，0号单元未用
	if (!HC)
	{
		return FALSE;
	}
	char* cd = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
	if (!cd)
	{
		return FALSE;
	}
	cd[n - 1] = '\0';//编码结束符
	for (int i = 1; i <= n; ++i)
	{
		int start = n - 1;//编码结束符位置
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c)//c为左子树
			{
				cd[--start] = '0';
			}
			else
			{
				cd[--start] = '1';
			}
		}
		HC[i] = (char*)malloc((n - start) * sizeof(char));//为第i个字符的赫夫曼编码的写入开辟空间
		if (!HC[i])
		{
			return FALSE;
		}
		strcpy(HC[i], &cd[start]);//写入第i个字符的赫夫曼编码
	}
	free(cd);
	return OK;
}//huffmanCoding

int Select(HuffmanTree HT, int n)//在从HT[1]到HT[n]中选择parent为0且weight最小的两个节点，其序号分别赋给s1和s2，且ASCLL码值s1<s2
{
	int min=0;
	for (int i = 1; i <= n; i++)//从i=1开始找到第一个没有父结点的结点，将其位置赋给min1
	{
		if (HT[i].parent == 0)
		{
			min = i;
		}
		break;
	}
	for (int i = 1; i <= n; i++)//逐步向后比较，找到最小的值赋给min1
	{
		if (HT[i].parent == 0 && HT[i].weight < HT[min].weight)
		{
			min = i;
		}
		else
		{
			if (HT[i].parent == 0 && HT[i].weight == HT[min].weight)//如果权值相等
			{
				if (HT[i].data&&HT[min].data)//如果都有数据则数据askll码小的排在前面
				{
					if (HT[i].data < HT[min].data)
					{
						min = i;
					}
				}
				else if (HT[i].data != 0 && HT[min].data != 0)//如果都没有数据则按序号大小从小到大排
				{
					if (i < min)
					{
						min = i;
					}
				}
				else if (HT[i].data != 0 && HT[min].data == 0)//有数据的排在没有数据的前面
				{
					min = i;
				}
			}
		}
	}
	HT[min].parent = 1;//将min1位置的父结点设为非零
	return min;
}

void preordertraverse(HuffmanTree HT, int n)
{
	SqStack S;
	InitStack(S, n);
	int Nownode = 2 * n - 1;
	int level = 1;/*节点所在层数*/
	int degree;/*节点的度数*/
	int temp;/*暂存Nownode的值*/
	while (Nownode || !StackEmpty(S))
	{
		if (Nownode != 0)
		{
			if (HT[Nownode].lchild && (HT[Nownode].rchild))
			{
				degree = 2;
			}
			else if (!HT[Nownode].lchild && !(HT[Nownode].rchild))
			{
				degree = 0;
			}
			else
			{
				degree = 1;
			}
			Push(S, Nownode);
			temp = Nownode;
			for (temp; HT[temp].parent != 0; temp = HT[temp].parent)
			{
				level++;
			}
			
			Nownode = HT[Nownode].lchild;
		}
		else
		{
			Pop(S, Nownode);
			Nownode = HT[Nownode].rchild;
		}
		level = 1;
	}
	
}

int main(int argc, char* argv[])
{
	if (argc != 3)//命令行参数输入错误
	{
		printf("ERROR_01");
		return OK;
	}
	HuffmanTree HT; HuffmanCode HC; unsigned int* w; char *v; int n;
	int len = strlen(argv[1]);
	if (len < 20)
	{
		printf("ERROR_02");
		return OK;
	}
	int count = 0;
	for (int i = 0; i < len; i++)//求命令行参数中字符种类数
	{
		for (int j = 0; j < i; j++)//判断第i个字符在前面是否出现过
		{
			if (*(argv[1] + j) == *(argv[1] + i))
			{
				i++;
				j = -1;
			}
			if (i == len)
			{
				count--;
				break;
			}
		}
		count++;
	}
	n = count;
	w = (unsigned int*)malloc(n * sizeof(int));
	v = (char*)malloc(n * sizeof(char));
	int count1 = 1;//记录字符出现的次数，存入w数组
	int count2 = 0;//记录存入w.数组的位置
	int count3=0;//记录存入v数组的位置
	int sign = 0;//判断第i个字符是否出现过的标志位，若出现过则为1，否则为0
	for (int i = 0; i <len; i++)
	{
			for (int k = 0; k < i; k++)//判断第i个字符在前面是否出现过
			{
				if (*(argv[1] + k) == *(argv[1] + i))//如果命令行参数第i个字符在前面已经出现过，则跳过第i个字符
				{
					sign = 1;
					break;
				}
			}
			if (sign == 0)//如果第i个字符未出现过
			{
				for (int j = i + 1; j < len; j++)//从第i个字符往后找求第i个字符出现的个数
				{
					if (*(argv[1] + i) == *(argv[1] + j))//计算第i个字符出现的次数即权值
					{
						count1++;
					}
				}
				w[count2++] = count1;
				v[count3++] = *(argv[1] + i);
				count1 = 1;
			}
			sign = 0;
	}
	HuffmanCoding(HT, HC, w, v, n);
	if (HuffmanCoding(HT, HC, w, v, n) == FALSE)
	{
		printf("ERROR_02");
		return OK;
	}
	preordertraverse(HT, n);
	for (int i = 0; i < len; i++)//编码
	{
		for (int j = 1; j <= n; j++)
		{
			if (HT[j].data == *(argv[1] + i))
			{
				printf("%s", HC[j]);
			}
		}
	}
	printf(" ");
	HTNode root = HT[2 * n - 1];
	int len2 = strlen(argv[2]);
	for (int i = 0; i <=len2; i++)//译码
	{
		if (i == strlen(argv[2]) && root.data != 0)
		{
			printf("%c",root.data);//将最后一个字符输出
			break;
		}
		if (i == strlen(argv[2]) && root.data == 0)
		{
			printf("\nERROR_03");
			return OK;
		}
		if (*(argv[2] + i) != '0'&&*(argv[2] + i) != '1')
		{
			printf("\nERROR_03");
			return OK;
		}
		if (*(argv[2] + i) == '0')
		{
			if (root.lchild == 0)//如果左子树不存在
			{
				printf("%c", root.data);
				root = HT[2 * n - 1];//指针回到根节点准备下一个字符的译码
				i--;
			}
			else
			{
				root = HT[root.lchild];
			}
		}
		else
		{
			if (root.rchild == 0)//如果右子树不存在
			{
				printf("%c", root.data);
				root = HT[2 * n - 1]; //指针回到根节点准备下一个字符的译码
				i--;
			}
			else
			{
				root = HT[root.rchild];//根节点指针向右子树移动
			}
		}
	}
	for (int i = 1; i <= n; i++)
	{
		free(HC[i]);
	}
	free(w);
	free(v);
	free(HT);
	free(HC);
	return OK;
}
