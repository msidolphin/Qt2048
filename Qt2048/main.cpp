#include <QApplication>
#include "gamewindow.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    /*
    int m[4][4] = {
        {4, 2, 0, 2},
        {4, 0, 2, 2},
        {4, 2, 0, 2},
        {4, 2, 0, 2}
    };
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            printf("%4d", m[i][j]);
        }
        printf("\n");
    }
    bool c[4][4] = {
        {false, false, false, false},
       {false, false, false, false},
        {false, false, false, false},
       {false, false, false, false}
    };
    for(int i = 0 ; i < ORDER ; ++i) {
        int j = 0;
        for(int k = 0 ; k < ORDER ; ++k) {
            //向前搜索第一个不为0的数
            while(k < ORDER && m[i][k] == 0) k++;
            if(k >= ORDER) {
                break;
            }
            if(k != j && m[i][j] == 0) {
                m[i][j] = m[i][k];
                m[i][k] = 0;
            }
            //判断死否和前面方块数值相等
            if(j > 0 && m[i][j] == m[i][j-1] && !c[i][j-1]) {
                m[i][j-1] *= 2;
                m[i][j] = 0;
                c[i][j-1] = true;
            }else {
                j++;
            }
        }
    }
    memset(c, 0, sizeof(bool) * 16);
    printf("\n");

    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            printf("%4d", m[i][j]);
        }
        printf("\n");
    }
    */

    QApplication a(argc, argv);
    GameWindow game;
    game.show();

    return a.exec();



}
