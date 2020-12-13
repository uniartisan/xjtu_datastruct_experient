//Find the shortest way.
#include<stdio.h>
#include<string.h>
#define MAX 20	//�ڵ�������
#define INFINITY 65535	//�޷�����ĳ���
#define TRUE 1
#define FALSE 0

typedef struct
{
	const char* ves[MAX];
	int vexnum;
	int Edge[MAX][MAX];	//�ڵ�Ȩֵ������
}MGraph;

typedef int PathMatrix[MAX][MAX];
typedef int ShortPathTable[MAX];

void initgraph(MGraph& G)//������֪������������ͼG
{
	G.vexnum = 13;//һ����13���ص�
	G.ves[0] = "����";
	G.ves[1] = "��ˮ˼Դ";
	G.ves[2] = "�ڷ���";
	G.ves[3] = "ͼ���";
	G.ves[4] = "��ѧ��¥";
	G.ves[5] = "�����";
	G.ves[6] = "����";
	G.ves[7] = "������";
	G.ves[8] = "������4";
	G.ves[9] = "������1";
	G.ves[10] = "������3";
	G.ves[11] = "������2";
	G.ves[12] = "��Ǩ��";
	for (int i = 0; i < G.vexnum; i++)
	{
		for (int j = 0; j < G.vexnum; j++)
		{
			G.Edge[i][j] = INFINITY;	//������ʼ��ֵ
		}
	}
	//��֪�����·�����ȣ�����ħ������
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

	for (int i = 0; i < G.vexnum; i++)//����ͼ���ڽӾ���������Խ��߶Գ�
	{
		for (int j = 0; j <= i; j++)
		{
			G.Edge[i][j] = G.Edge[j][i];
		}
	}
}

//G:���룬���й�Gͼ�����·������
//v0:���룬��������㵽v0�����·��
//P:��������P[v][w]ΪTRUE����wΪv0��v�����·���ϵĶ���
//D:���������������㵽v0���·�����ȵ�����
void ShortestPath(MGraph G, int v0, PathMatrix& P, ShortPathTable& D)
{
	int final[MAX];//ȷ�������Ƿ���������·���У��ݴ�
	int vj = 0;//vj������ʱ������·�����յ�
	for (int i = 0; i < G.vexnum; i++)//��ʼ��P�����D����
	{
		final[i] = FALSE;
		D[i] = G.Edge[v0][i];
		for (int j = 0; j < G.vexnum; j++)
		{
			P[i][j] = FALSE;
		}
		if (D[i] < INFINITY)//v0��i����һ��·������vo��i��Ϊ·���ϵĶ���
		{
			P[i][v0] = TRUE;
			P[i][i] = TRUE;
		}
	}
	D[v0] = 0;//���㵽�����·������Ϊ��
	final[v0] = TRUE;//��v0����final����
	for (int i = 1; i < G.vexnum; i++)    //��ѭ����ÿ����һ��v��v0�����·��
	{
		int min = INFINITY;
		for (int j = 0; j < G.vexnum; j++)// ��ǰ�����·�����յ�vj
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
		final[vj] = TRUE;    // ������vj����final
		for (int k = 0; k < G.vexnum; k++)   // ���µ�ǰ�����·��������
		{
			if (!final[k] && (min + G.Edge[vj][k] < D[k]))
			{
				D[k] = min + G.Edge[vj][k];
				for (int j = 0; j < G.vexnum; j++)
				{
					P[k][j] = P[vj][j]; // ����v0��k�����ȵ�vj
				}
				P[k][k] = TRUE;//���յ�k������·����
			}
		}
	}
}
int main(int argc, const char* argv[])
{
	if (argc != 3)//�����в�������
	{
		printf("ERROR_01");
		return FALSE;
	}
	int v1 = INFINITY;//v1,v2�ֱ�ָ����·���������յ㣬��ֵΪINFINITY��ʾ���v1��v2������ͼG�Ķ��㼯��
	int v2 = INFINITY;
	MGraph Grapth;
	initgraph(Grapth);//G��ʼ����ֵ
	for (int i = 0; i < Grapth.vexnum; i++)//�ҵ���Ӧλ�ýڵ�
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
	if (v1 == INFINITY || v2 == INFINITY)//��ͼG��δ�ҵ���֮ƥ��Ķ��㣬�����������
	{
		printf("ERROR_02");
		return FALSE;
	}
	PathMatrix Path_M;
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			Path_M[i][j] = 0;
		}
	}//��ʼ����ֵ
	ShortPathTable D_ShortPath;
	for (int i = 0; i < MAX; i++) {
		D_ShortPath[i] = 0;
	}//��ʼ����ֵ
	ShortestPath(Grapth, v1, Path_M, D_ShortPath);
	printf("%s", Grapth.ves[v1]);
	while (v1 != v2)//���·��
	{
		for (int i = 0; i < Grapth.vexnum; i++)
		{
			if (Path_M[v2][i] == TRUE && Grapth.Edge[v1][i] != INFINITY)//���i������v1��v2��·���У�����i��·������һ������v1���ڣ���i��Ϊ��һ������
			{
				printf("->%s", Grapth.ves[i]);
				Grapth.Edge[v1][i] = Grapth.Edge[i][v1] = INFINITY;//�ϵ�i��v1���ڽӹ�ϵ
				v1 = i;//v1����
				break;
			}
		}
	}
	printf("\n");
	printf("%d", D_ShortPath[v2]);//���·������
	return TRUE;
}