//Find the shortest way.
#include<stdio.h>
#include<string.h>
#define MAX 20	//节点最大个数
#define INFINITY 65535	//无法到达的长度
#define TRUE 1
#define FALSE 0

typedef struct
{
	const char* ves[MAX];
	int vexnum;
	int Edge[MAX][MAX];	//节点权值（距离
}MGraph;

typedef int PathMatrix[MAX][MAX];
typedef int ShortPathTable[MAX];

void initgraph(MGraph& G)//根据已知条件构造无向图G
{
	G.vexnum = 13;//一共有13个地点
	G.ves[0] = "北门";
	G.ves[1] = "饮水思源";
	G.ves[2] = "腾飞塔";
	G.ves[3] = "图书馆";
	G.ves[4] = "教学主楼";
	G.ves[5] = "活动中心";
	G.ves[6] = "南门";
	G.ves[7] = "宪梓堂";
	G.ves[8] = "传送门4";
	G.ves[9] = "传送门1";
	G.ves[10] = "传送门3";
	G.ves[11] = "传送门2";
	G.ves[12] = "西迁馆";
	for (int i = 0; i < G.vexnum; i++)
	{
		for (int j = 0; j < G.vexnum; j++)
		{
			G.Edge[i][j] = INFINITY;	//变量初始赋值
		}
	}
	//已知结点间的路径长度（不是魔鬼数字
	G.Edge[0][1] = 18;
	G.Edge[0][10] = 22;
	G.Edge[1][2] = 19;
	G.Edge[1][9] = 27;
	G.Edge[2][10] = 4;
	G.Edge[2][8] = 32;
	G.Edge[2][3] = 23;
	G.Edge[3][4] = 15;
	G.Edge[3][9] = 4;
	G.Edge[3][11] = 4;
	G.Edge[4][5] = 21;
	G.Edge[4][7] = 8;
	G.Edge[5][6] = 30;
	G.Edge[6][7] = 14;
	G.Edge[6][11] = 21;
	G.Edge[6][12] = 20;
	G.Edge[7][8] = 4;
	G.Edge[11][12] = 43;

	for (int i = 0; i < G.vexnum; i++)//无向图的邻接矩阵关于主对角线对称
	{
		for (int j = 0; j <= i; j++)
		{
			G.Edge[i][j] = G.Edge[j][i];
		}
	}
}

//G:输入，求有关G图的最短路径问题
//v0:输入，求各个顶点到v0的最短路径
//P:输出，如果P[v][w]为TRUE，则w为v0到v的最短路径上的顶点
//D:输出，储存各个顶点到v0最短路径长度的数组
void ShortestPath(MGraph G, int v0, PathMatrix& P, ShortPathTable& D)
{
	int final[MAX];//确定顶点是否包含在最短路径中，暂存
	int vj = 0;//vj用来暂时存放最短路径的终点
	for (int i = 0; i < G.vexnum; i++)//初始化P矩阵和D数组
	{
		final[i] = FALSE;
		D[i] = G.Edge[v0][i];
		for (int j = 0; j < G.vexnum; j++)
		{
			P[i][j] = FALSE;
		}
		if (D[i] < INFINITY)//v0与i存在一条路径，则vo与i必为路径上的顶点
		{
			P[i][v0] = TRUE;
			P[i][i] = TRUE;
		}
	}
	D[v0] = 0;//顶点到自身的路径长度为零
	final[v0] = TRUE;//将v0加入final数组
	for (int i = 1; i < G.vexnum; i++)    //主循环，每次求一个v到v0的最短路径
	{
		int min = INFINITY;
		for (int j = 0; j < G.vexnum; j++)// 求当前的最短路径的终点vj
		{
			if (!final[j])
			{
				if (D[j] < min)
				{
					vj = j;
					min = D[j];
				}
			}
		}
		final[vj] = TRUE;    // 将顶点vj加入final
		for (int k = 0; k < G.vexnum; k++)   // 更新当前的最短路径及长度
		{
			if (!final[k] && (min + G.Edge[vj][k] < D[k]))
			{
				D[k] = min + G.Edge[vj][k];
				for (int j = 0; j < G.vexnum; j++)
				{
					P[k][j] = P[vj][j]; // 若从v0到k则首先到vj
				}
				P[k][k] = TRUE;//将终点k包含在路径中
			}
		}
	}
}
int main(int argc, const char* argv[])
{
	if (argc != 3)//命令行参数错误
	{
		printf("ERROR_01");
		return FALSE;
	}
	int v1 = INFINITY;//v1,v2分别指所求路径的起点和终点，初值为INFINITY表示最初v1与v2不属于图G的顶点集合
	int v2 = INFINITY;
	MGraph Grapth;
	initgraph(Grapth);//G初始化赋值
	for (int i = 0; i < Grapth.vexnum; i++)//找到对应位置节点
	{
		if (strcmp(Grapth.ves[i], argv[1]) == 0)
		{
			v1 = i;
		}
		if (strcmp(Grapth.ves[i], argv[2]) == 0)
		{
			v2 = i;
		}
	}
	if (v1 == INFINITY || v2 == INFINITY)//在图G中未找到与之匹配的顶点，地名输入错误
	{
		printf("ERROR_02");
		return FALSE;
	}
	PathMatrix Path_M;
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			Path_M[i][j] = 0;
		}
	}//初始化赋值
	ShortPathTable D_ShortPath;
	for (int i = 0; i < MAX; i++) {
		D_ShortPath[i] = 0;
	}//初始化赋值
	ShortestPath(Grapth, v1, Path_M, D_ShortPath);
	printf("%s", Grapth.ves[v1]);
	while (v1 != v2)//输出路径
	{
		for (int i = 0; i < Grapth.vexnum; i++)
		{
			if (Path_M[v2][i] == TRUE && Grapth.Edge[v1][i] != INFINITY)//如果i包含在v1到v2的路径中，并且i与路径中上一个顶点v1相邻，则i必为下一个顶点
			{
				printf("->%s", Grapth.ves[i]);
				Grapth.Edge[v1][i] = Grapth.Edge[i][v1] = INFINITY;//断掉i与v1的邻接关系
				v1 = i;//v1后移
				break;
			}
		}
	}
	printf("\n");
	printf("%d", D_ShortPath[v2]);//输出路径长度
	return TRUE;
}