#include "head.h"

//linux�±�����Ӳ���-Dsys_linux

int currentUser;//0�����壬1������
int mode;//1�����˶�ս��2���˻���ս�����ִ�ڣ���3���˻���ս�����ִ�ף���4��������ս

int main()
{
    char a[MAXLENGTH];//������ȫ����char a[MAXLENGTH]�����������������

    //��ʼ��
    #ifndef sys_linux//in WIN version
    system("color 70");
    #endif
    initial();
    //����Ԥ�����ӿ������ٶ�
    initialPatternTable();
    initialScoreTable();

    //ѡ��ģʽ
    printf("Welcome to gobang game(Work of ZengHongbin).(*^__^*)\n");
    printf("Please choose game mode:\n");
    printf("1: pvp\n2: pve(player go first)\n3: pve(AI go first)\n4: AI vs AI\n");
    while(getLine(a) != 2 || a[0] < '1' || a[0] > '4')//Ҫ��������ȷ
        printf("Wrong input.\n");
    mode = a[0] - '0';

    re: displayBoard();//չʾ����

    while(1)
    {
        displayWord();//չʾһЩ����
        operate();//��һ���AI���в���
        displayBoard();
        if((mode == 2 && currentUser == 1) || (mode == 3 && currentUser == 0) || mode == 4)//AI���ʱ��������
        {
            char x;
            int y;
            translateIntToChar(&x, &y, recordPoint[pIndex].x, recordPoint[pIndex].y);
            printf("%c%d\n", x, y);

            if(mode == 4)//������ս����ͣ
            {
                #ifdef sys_linux
                int l;
                printf("press ENTER to continue or quit to quit.\n");
                while(1)
                {
                    l = getLine(a);
                    if(l == 1 && a[0] == '\n')
                        break;
                    else if(isEqual_string(a, "quit\n"))
                        exit(0);
                    printf("press ENTER to continue or quit to quit.\n");
                }
                #else
                system("pause");
                #endif // sys_linux
            }
        }

        if(pIndex == SIZE*SIZE)//�����������
        {
            printf("All places have been placed, the game ended in a draw.\n");
            restart();
            goto re;
        }
        if(isVictorious(currentUser))
        {
            printf("Player%d win!!!\n", currentUser+1);
            restart();
            goto re;
        }

        if(currentUser == 0 && (mode == 1 || mode == 2 ) && pIndex >= 11 && isForbiddenMove(recordPoint[pIndex].x, recordPoint[pIndex].y, currentUser))//�жϺ����Ƿ��������
        {
            printf("Player1, a forbidden move will be made, do you want to change your move or get lost(y/n)?\n");
            setRecordBoard(recordPoint[pIndex].x, recordPoint[pIndex].y, 0);
            --pIndex;
            while(getLine(a) != 2 || (a[0] != 'y' && a[0] != 'n'))//Ҫ��������ȷ
                printf("Wrong input.\n");
            if(a[0] == 'y')
                goto re;
            else
            {
                printf("Player2 win!!!\n");
                restart();
                goto re;
            }
        }

        changeCurrentUser();
    }
    return 0;
}

void operate()//����һ���AI���в���
{
    if(mode == 1)
        player();
    else if(mode == 2)
    {
        if(currentUser == 0)
            player();
        else
            AI();
    }
    else if(mode == 3)
    {
        if(currentUser == 0)
            AI();
        else
            player();
    }
    else if(mode == 4)
        AI();
}

void changeCurrentUser()
{
    currentUser = !currentUser;
}
