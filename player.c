#include "head.h"

void player()
{
    while(!operate_player())
        ;
}

int operate_player()//��ҽ��в���
{
    char a[MAXLENGTH];
    int x, y, l;

    if(getLine(a) == MAXLENGTH-1)//�������
    {
        printf("Error: wrong input.\n");
        if(a[MAXLENGTH-1] != '\n')//��ջ�����
            for(l = getLine(a); a[l-1] != '\n'; l = getLine(a))
                ;
        return 0;
    }
    if(isEqual_string(a, "help\n"))
    {
        printf("useful command: recall, restart, exchange, quit\n");
        return 0;
    }
    if(isEqual_string(a, "restart\n"))
    {
        initial();
        displayBoard();
        displayWord();
        operate();
        return 1;
    }
    if(isEqual_string(a, "recall\n"))
    {
        int t;
        if((t = recall()) == 1)//������һ��
        {
            changeCurrentUser();
            displayBoard();
            displayWord();
            operate();
            return 1;
        }
        if(t == 2)//����������
        {
            displayBoard();
            displayWord();
            operate();
            return 1;
        }
        return 0;
    }
    if(isEqual_string(a, "exchange\n"))
    {
        setRecordBoard(-1, -1, 0);
        changeCurrentUser();
        displayBoard();
        displayWord();
        operate();
        return 1;
    }
    if(isEqual_string(a, "quit\n"))
    {
        exit(0);
    }
    if(a[2] != '\n' && a[3] != '\n')
    {
        printf("Error: wrong input.\n");
        return 0;
    }
    if(!translateCharToInt(a, &x, &y))//�������
    {
        printf("Error: wrong input.\n");
        return 0;
    }
    return(place(x, y, currentUser));//��������
}

int recall()//���س��صĲ�����0��ʾʧ��
{
    if(pIndex == 0)
    {
        printf("No piece is placed.\n");
        return 0;
    }
    if(mode == 1)
    {
        setRecordBoard(recordPoint[pIndex].x, recordPoint[pIndex].y, 0);
        updatePattern(recordPoint[pIndex].x, recordPoint[pIndex].y);
        updateFlag(recordPoint[pIndex].x, recordPoint[pIndex].y, -1);
        --pIndex;
        return 1;
    }
    else//�˻���ս��Ҫ��������
    {
        if(pIndex <= 1)
        {
            printf("No piece is placed.\n");
            return 0;
        }
        setRecordBoard(recordPoint[pIndex].x, recordPoint[pIndex].y, 0);
        updatePattern(recordPoint[pIndex].x, recordPoint[pIndex].y);
        updateFlag(recordPoint[pIndex].x, recordPoint[pIndex].y, -1);
        --pIndex;
        setRecordBoard(recordPoint[pIndex].x, recordPoint[pIndex].y, 0);
        updatePattern(recordPoint[pIndex].x, recordPoint[pIndex].y);
        updateFlag(recordPoint[pIndex].x, recordPoint[pIndex].y, -1);
        --pIndex;
        return 2;
    }
}

int isEqual_string(char a[], char b[])//�ж��ַ���a��b�Ƿ����
{
    int i;
    for(i = 0; a[i] && b[i]; ++i)
        if(a[i] != b[i])
            return 0;
    return (a[i] == b[i]);
}

int translateCharToInt(char a[3], int *x, int *y)//�����������ת���ɳ��������꣬����A15->(1,1)�����������Ƿ�Ϸ�
{
    /*
        ����������ϵ�������������
        O--y
        |
        x

        (1,1)   (1,15)

        (15,1)  (15,15)
    */

    int right = 0;
    if((a[0] >= 'a' && a[0] <= 'o') || (a[0] >= 'A' && a[0] <= 'O'))
    {
        *y = a[0] - ((a[0]>='a')?'a':'A') + 1;
        ++right;
        if(a[1] >= '0' && a[1] <= '9')
        {
            if(a[2] >= '0' && a[2] <= '9')
                *x =16 - ((a[1] - '0')*10 + a[2] - '0');
            else
                *x =16 - (a[1] - '0');
            if(*x <= 15 && *x >= 1)
                ++right;
        }
        return (right == 2);
    }
    if(a[0] >= '0' && a[0] <= '9')
    {
        if(a[1] >= '0' && a[1] <= '9')
            *x = 16 - ((a[0] - '0')*10 + a[1] - '0');
        else
            *x = 16 - (a[0] - '0');
        if(*x <= 15 && *x >= 1)
            ++right;
    }
    if(a[1] >= 'a' && a[1] <= 'o')
    {
        *y = a[1] - 'a' + 1;
        ++right;
    }
    if(a[1] >= 'A' && a[1] <= 'O')
    {
        *y = a[1] - 'A' + 1;
        ++right;
    }
    if(a[2] >= 'a' && a[2] <= 'o')
    {
        *y = a[2] - 'a' + 1;
        ++right;
    }
    if(a[2] >= 'A' && a[2] <= 'O')
    {
        *y = a[2] - 'A' + 1;
        ++right;
    }
    return (right == 2);
}