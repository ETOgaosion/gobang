#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 15//���̴�С
#define MAXTIME 15000//�����ʱ��
#define MAXLENGTH 50//getLine��������ַ���
#define DEPTH 6//�������
#define initialAlpha -100000000//alpha-beta�����ĳ�ʼalpha��������С
#define initialBeta   100000000//alpha-beta�����ĳ�ʼbeta�������ܴ�
#define MAXN 40//ÿ������������������ڽ�����������Ϊ̫����ĵ��ǱȽϲ��

#define WHITE 0
#define BLACK 1

#define EMPTY 0
#define WHITEPIECE 1//����
#define BLACKPIECE 2//����
#define OUTSIDE 3//��������
#define WIN 10000

#define L1 0//��һ
#define D2 1//����
#define L2 2//���
#define D3 3//����
#define L3 4//����
#define D4 5//����
#define L4 6//����
#define L5 7//����

struct point//��������
{
    int x;
    int y;
};

struct node{//�������꼰������
    int x;
    int y;
    int value;
};

//ȫ�ֱ���
extern int currentUser;
extern int mode;//ģʽ

extern const int dx[5];//�������������ڱ���
extern const int dy[5];

extern int recordBoard[SIZE+1][SIZE+1];//���̼�¼��0���գ�1�����壬2������
extern struct point recordPoint[SIZE*SIZE+1];//�����µĵ�i���������
extern int pIndex;////������¼�µĵ�������1��ʼ
extern int patternTable[2][65536];//���ͱ�����key��ֵ��������
extern int pattern[2][SIZE+1][SIZE+1][5];//���ͼ�¼��
extern int flag[SIZE+1][SIZE+1];//��Χ��2Ϊ�뾶�����5*5�����е�����������

extern int scoreTable[8][8][8][8];//��ֵ������һ�����ĸ���������Ͳ��ҷ�ֵ

//����

//gobang.c
int main();
void operate();//����һ���AI���в���
void changeCurrentUser();

//AI.c
void AI();
void maxmin(int user);//��alpha-beta��֦�ĸ�ֵ�������
double alphabeta(int user, int depth, int alpha, int beta);//alpha-beta����
int cmp(struct node a, struct node b);//����ֵ���������ͬ������������Զ����������
void mySwap(struct node p[], int a, int b);
void quicksort(struct node p[], int l, int r, int (*cmp)(struct node, struct node));//�Կ��ýڵ㰴��ֵ��������
int evaluateMove(int x, int y, int user);//�����ӵ��������
int isPatternExisting(int user, int x, int y, int type);//ĳ�����Ƿ����ĳ������
int generate(int user, struct node piece[]);//���ɰ���ֵ����Ŀ����ӵ㣬����piece���飬���������С
int patternsToScore(int d1, int d2, int d3, int d4);//���ĸ���������ͽ�������
void initialScoreTable();
double score(int user);//���ض�AI�ľ�������
int distance(int x, int y);//������(7,8)�ľ���

//player.c
void player();
int operate_player();//��ҽ��в���
int recall();//���س��صĲ�����0��ʾʧ��
int isEqual_string(char a[], char b[]);//�ж��ַ���a��b�Ƿ����
int translateCharToInt(char a[3], int *x, int *y);//�����������ת���ɳ��������꣬����A15->(1,1)�����������Ƿ�Ϸ�

//others.c
int getLine(char a[]);//get a line inputed or a (MAXLENGTH-1) long string, return '\0''s index
int isPieceOfUser(int x, int y, int user);
void initial();
int isVictorious(int user);
void restart();//���¿�ʼ
int subrestart();//Ҫ��������ȷ
int isInBoard(int x, int y);
int isValidPiece(int x, int y, int user);//�Ƿ����������Լ�user������
int isAvailablePlace(int x, int y, int user);//�Ƿ����������ڵġ��յġ��ǽ��ֵ�
int isUsablePlace(int x, int y);//�Ƿ����������ڵġ��ǽ��ֵ�

//board.c
void initialPatternTable();
void updateFlag(int x, int y, int state);//����(x, y)��Χ��2Ϊ�뾶�����5*5���ε�flag��������(x, y)
void updatePattern(int x, int y);//����(x, y)��Χ�ĸ�����32��������ͣ�������(x, y)
int getKey(int x, int y, int d);//��ȡ���ڷ���d�ϵ�key
int keyToPattern(int user, int key);//��keyת��������
int place(int x, int y, int user);//���ӣ�ʧ�ܷ���0
void setRecordBoard(int x, int y, int state);//(0,0)����ʼ����(-1, -1)�ڰ׽���
void displayWord();
void displayBoard();
void translateIntToChar(char *ansx, int *ansy, int x, int y);//����(1,1)->A15

//forbiddenMove.c
int isForbiddenMove(int x, int y, int user);
int doubleThree(int x, int y, int user);
int doubleFour(int x, int y, int user);
int three(int x, int y, int user);//return number of living three
struct point four(int x, int y, int user);//s.x: number of living four, s.y: number of die four
int five(int x, int y, int user);//-1 for less than five, 0 for five, 1 for more than five

#endif // HEAD_H_INCLUDED