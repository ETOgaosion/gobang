#include "head.h"

int scoreTable[8][8][8][8];//��ֵ������һ�����ĸ���������Ͳ��ҷ�ֵ

void AI()
{
    if(pIndex == 0 || pIndex == 1)//��һ�ӿ�����������
    {
        if(recordBoard[7][8] != 0)//����Ѿ�����
            place(8, 8, currentUser);
        else
            place(7, 8, currentUser);
    }
    else
        maxmin(currentUser);
}

void maxmin(int user)//��alpha-beta��֦�ĸ�ֵ�������
{
    struct node piece[SIZE*SIZE+1];//�������ɵ����ӵ�
    int n = generate(user, piece);//���ɰ���ֵ����Ŀ����ӵ�
    int i, x, y;
    int alpha = initialAlpha, t;
    int begin = clock();
    int mx = 1, my = 1;
    for(i = 1; i <= n; ++i)
    {
        x = piece[i].x;
        y = piece[i].y;
        setRecordBoard(x, y, (user==0)?1:2);//��������
        updateFlag(x, y, 1);
        updatePattern(x, y);
        t = -alphabeta(!user, DEPTH-1, initialAlpha, -alpha);//��ֵ�������
        setRecordBoard(x, y, 0);
        updatePattern(x, y);
        updateFlag(x, y, -1);
        if(t > alpha)//�ҵ����ŵĽ�
        {
            alpha = t;
            mx = x;
            my = y;
        }
        if(clock()-begin >= MAXTIME)//����Ѿ���ʱ�������ǰ���Ž�
            break;
    }

    place(mx, my, user);
}

double alphabeta(int user, int depth, int alpha, int beta)
{
    if(depth == 0)//Ҷ�ӽڵ㣬���ؾ����ֵ
        return score(user);
    struct node piece[SIZE*SIZE+1];
    int n = generate(user, piece);
    int i, x, y;
    int t;

    if(piece[1].value >= 2000)//���ڱ�ʤ�߷�
        return WIN;

    for(i = 1; i <= n; ++i)
    {
        x = piece[i].x;
        y = piece[i].y;
        setRecordBoard(x, y, (user==0)?1:2);
        updateFlag(x, y, 1);
        updatePattern(x, y);
        t = -alphabeta(!user, depth-1, -beta, -alpha);
        setRecordBoard(x, y, 0);
        updatePattern(x, y);
        updateFlag(x, y, -1);
        if(t > alpha)
            alpha = t;
        if(alpha >= beta)//alpha-beta��֦
            break;
    }
    return alpha;
}

int cmp(struct node a, struct node b)//����ֵ���������ͬ������������Զ����������
{
    if(b.value > a.value)
        return 1;
    else if(b.value < a.value)
        return -1;
    else
        return distance(a.x, a.y) - distance(b.x, b.y);
}

void mySwap(struct node p[], int a, int b)
{
    struct node t = p[a];
    p[a] = p[b];
    p[b] = t;
}

void quicksort(struct node p[], int l, int r, int (*cmp)(struct node, struct node))//�Կ��ýڵ㰴��ֵ��������
{
    int mid = (l+r-1)/2;
    int last = l, i = l+1;
    if(l >= r-1)
        return;
    mySwap(p, l, mid);
    for(; i < r; ++i)
        if((*cmp)(p[i], p[l]) < 0)
            mySwap(p, ++last, i);
    mySwap(p, last, l);
    quicksort(p, l, last, cmp);
    quicksort(p, last+1, r, cmp);
}

int evaluateMove(int x, int y, int user)//�����ӵ��������
{
	int score[2];
	score[user] = scoreTable[pattern[user][x][y][1]] [pattern[user][x][y][2]] [pattern[user][x][y][3]] [pattern[user][x][y][4]];//ͨ���ĸ���������Ͳ��ҷ�ֵ
	score[!user] = scoreTable[pattern[!user][x][y][1]] [pattern[!user][x][y][2]] [pattern[!user][x][y][3]] [pattern[!user][x][y][4]];

	if (score[user] >= 200 || score[!user] >= 200)//����ɱ��
		return (score[user]>=score[!user])?score[user]*2:score[!user];//�ڶԷ����ܳ���������������Լ���ɱ��
	else
		return score[user]*2 + score[!user];//���ط�ֵ�����Ƚ���
}

int generate(int user, struct node piece[])//���ɰ���ֵ����Ŀ����ӵ㣬����piece���飬���������С
{
    int i, j, t;
    int n = 0;
    for(i = 1; i <= SIZE; ++i)
    {
        for(j = 1; j <= SIZE; ++j)
        {
            if(flag[i][j] && isAvailablePlace(i, j, user))//ֻѡ�����������ӵķǽ��ֵ�
            {
                if( (t = evaluateMove(i, j, user)) > 0)//���ٴ�����������һ�����º󲻿�����������û��
                {
                    ++n;
                    piece[n].x = i;
                    piece[n].y = j;
                    piece[n].value = t;
                }
            }
        }
    }
    quicksort(piece, 1, n+1, cmp);
    if(piece[1].value >= 2000)//���ڱ�ʤ�߷�
        return 1;
    if (piece[1].value == 1200)//�Է����ڻ���
	{
		for (i = 2; i <= n; ++i)//Ѱ�һ�����
			if (piece[i].value != 1200)
                break;
		for (; i <= n; ++i)//Ѱ���Լ��ͶԷ������ĵ�
		{
			if(isPatternExisting(user, piece[i].x, piece[i].y, D4) | isPatternExisting(!user, piece[i].x, piece[i].y, D4))
                ;
            else
                break;
		}
		return (i-1 > MAXN)?MAXN:n;//�Է���ɱ�У�ֱ�ӷ��أ�����������
	}
    return (n > MAXN)?MAXN:n;//���ֻ����MAXN����
}

int isPatternExisting(int user, int x, int y, int type)//ĳ�����Ƿ����ĳ������
{
    return (pattern[user][x][y][1] == type) | (pattern[user][x][y][2] == type) | (pattern[user][x][y][3] == type) | (pattern[user][x][y][4] == type);
}

int patternsToScore(int d1, int d2, int d3, int d4)//���ĸ���������ͽ�������
{
	int t[8] = {0};//L1,D2,L2,D3,L3,D4,L4,L5��Ӧ��������
	int s = 0, i;
	const int weight[6] = {0, 2, 5, 5, 12, 12};//L1,D2,L2,D3,L3,D4
	++t[d1];
	++t[d2];
	++t[d3];
	++t[d4];

	if (t[L5] > 0)//���Ļ��߻���
		return 5000;
	if (t[L4] > 0 || t[D4] > 1)//����
		return 1200;
	if (t[D4] > 0 && t[L3] > 0)//�������߻��
		return 1000;
	if (t[L3] > 1)//˫��������Ӻ��γ�˫��
		return 200;

	for (i = D2; i <= D4; i++)//����Ȩֵ�Ʒ�
		s += weight[i] * t[i];
	return s;
}

void initialScoreTable()
{
    int d1, d2, d3, d4;
    for(d1 = 0; d1 <= L5; ++d1)
        for(d2 = 0; d2 <= L5; ++d2)
            for(d3 = 0; d3 <= L5; ++d3)
                for(d4 = 0; d4 <= L5; ++d4)
                    scoreTable[d1][d2][d3][d4] = patternsToScore(d1, d2, d3, d4);
}

double score(int user)//���ض�AI�ľ�������
{
    #if(DEPTH%2 != 0)//Ϊ�˷��ض�AI�ľ������֣����������Ϊ����Ҫ�ı�user
    user = !user;
    #endif
	int userType[8] = {0};//L1,D2,L2,D3,L3,D4,L4,L5��Ӧ��������
	int enemyType[8] = {0};
	int t = 0, i, j;
	int userScore = 0, enemyScore = 0;
    const int weight[8] = {0, 2, 12, 18, 96, 144, 800, 1200};//L1,D2,L2,D3,L3,D4,L4,L5��Ȩֵ

	for (i = 1; i < SIZE; ++i)
	{
		for (j = 1; j < SIZE; ++j)
		{
			if (flag[i][j] && (!recordBoard[i][j]))
			{
				t = userType[D4];
				++userType[pattern[user][i][j][1]];
				++userType[pattern[user][i][j][2]];
				++userType[pattern[user][i][j][3]];
				++userType[pattern[user][i][j][4]];
				++enemyType[pattern[!user][i][j][1]];
				++enemyType[pattern[!user][i][j][2]];
				++enemyType[pattern[!user][i][j][3]];
				++enemyType[pattern[!user][i][j][4]];

				if (userType[D4] - t >= 2)//������������ģ���ô����һ������
				{
					userType[D4] -= 2;
					userType[L4]++;
				}
			}
		}
	}

	for (i = 1; i < 8; ++i)
	{
		userScore += userType[i] * weight[i];
		enemyScore += enemyType[i] * weight[i];
	}

	return userScore*1.2 - enemyScore;//�����ȷ�����Ҫ
}

int distance(int x, int y)//������(7,8)�ľ���
{
    int xd, yd;
    xd = x - 7;
    yd = y - 8;
    if(xd < 0)
        xd = -xd;
    if(yd < 0)
        yd = -yd;
    return xd + yd;
}