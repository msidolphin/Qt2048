#include "gamewidget.h"
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QRectF>
#include <QPalette>
#include <QPaintEvent>
#include <QMap>
#include <QEvent>

#include <QDebug>

#include <ctime>
#include <random>

#include <iostream>

using namespace std;


GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    srand(time(nullptr));

    //分数初始化为0
    score = 0;

    //初始化背景颜色
    backgroundColor.insert(QString::number(0),      QColor::fromRgb(0xab, 0xa5, 0x8d));
    backgroundColor.insert(QString::number(2),      QColor::fromRgb(0xee, 0xe4, 0xda));
    backgroundColor.insert(QString::number(4),      QColor::fromRgb(0xed, 0xe0, 0xc8));
    backgroundColor.insert(QString::number(8),      QColor::fromRgb(0xf2, 0xb1, 0x79));
    backgroundColor.insert(QString::number(16),     QColor::fromRgb(0xf5, 0x95, 0x63));
    backgroundColor.insert(QString::number(32),     QColor::fromRgb(0xf6, 0x7c, 0x5f));
    backgroundColor.insert(QString::number(64),     QColor::fromRgb(0xf6, 0x5e, 0x3b));
    backgroundColor.insert(QString::number(128),    QColor::fromRgb(0xed, 0xcf, 0x72));
    backgroundColor.insert(QString::number(256),    QColor::fromRgb(0xed, 0xcc, 0x61));
    backgroundColor.insert(QString::number(512),    QColor::fromRgb(0x99, 0xcc, 0x00));
    backgroundColor.insert(QString::number(1024),   QColor::fromRgb(0xaa, 0x66, 0xcc));
    backgroundColor.insert(QString::number(2048),   QColor::fromRgb(0x99, 0x33, 0xcc));

    //初始化border为0
    memset(border, 0, sizeof(int) * 16);



    //随机生成两个2个4
    random(2, 4);

    resize(400, 400);
    //先固定大小
    setFixedSize(this->width(), this->height());
}

GameWidget::~GameWidget()
{

}


/**
 * 绘制
 * @brief GameWidget::paintEvent
 * @param event
 */
void GameWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    QBrush brush(QColor::fromRgb(141, 121, 81));
    painter.setBrush(brush);
    painter.drawRect(2, 2, width() - 4, height() - 4);

    w = (width() - 4) - 25;
    w /= 4;
    h = w;

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);

    painter.setFont(font);

    //画笔设置为空，不绘制边框
    painter.setPen(Qt::NoPen);
    int n = 4;

    for(int i = 0 ; i < n ; ++i) {
        for(int j = 0 ; j < n ; ++j) {
            //绘制背景
            brush.setColor(backgroundColor.value(QString::number(border[i][j])));
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), 20, 20);
            if(border[i][j] != 0) {
                //绘制文字
                painter.setPen(QColor::fromRgb(0, 0, 0));
                painter.drawText(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), Qt::AlignCenter, QString::number(border[i][j]));
                painter.setPen(Qt::NoPen);
            }
        }
    }
}

/**
 * 随机数
 * @brief GameWidget::random
 */
void GameWidget::random(int count, int value)
{
    int i = 0;
    int j = 0;

    bool hasZero = false;

    //判断是否存在0
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            if(border[i][j] == 0) {
                hasZero = true;
            }
        }
    }

    if(hasZero) {
        while(count) {
            i = rand() % 4;
            j = rand() % 4;
            if(border[i][j] == 0) {
                count--;
                border[i][j] = value;
            }
        }
    }
}

/**
 * 移动border矩阵，重写绘制
 * @brief GameWidget::move
 * @param direct
 */
void GameWidget::move(Direct direct)
{
    int temp[4];

    memset(temp, 0, 4 * sizeof(int));

    int num = 0;

    for(int i = 0 ; i < 4 ; ++i) {
        num = 0;
        int index = 0;
        bool flag = false;
        switch (direct){
        case Direct::Up:
            for(int j = 0 ; j < 4 ; ++j) {
                if(border[j][i] != 0) {
                    temp[index++] = border[j][i];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                merge(temp, num);
                for(int j = 0 ; j < 4 ; ++j) {
                    if(temp[j] != 0) {
                        border[j][i] = temp[j];
                    }else {
                        border[j][i] = 0;
                    }
                }
                break;
            }
            break;
        case Direct::Down:
            for(int j = 3 ; j >= 0 ; --j) {
                if(border[j][i] != 0) {
                    temp[index++] = border[j][i];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                merge(temp, num);
                for(int j = 3 ; j >= 0 ; --j) {
                    if(temp[3 - j] != 0) {
                        border[j][i] = temp[3 - j];
                    }else {
                        border[j][i] = 0;
                    }
                }
            }
            break;
        case Direct::Left:
            for(int j = 0 ; j < 4 ; ++j) {
                if(border[i][j] != 0) {
                    temp[index++] = border[i][j];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                merge(temp, num);
                //由于是向左，所以是以列的正向
                for(int j = 0 ; j < 4 ; ++j) {
                    if(temp[j] != 0) {
                        border[i][j] = temp[j];
                    }else {
                        border[i][j] = 0;
                    }
                }
            }
            break;
        case Direct::Right:
            for(int j = 3 ; j >= 0 ; --j) {
                if(border[i][j] != 0) {
                    temp[index++] = border[i][j];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                merge(temp, num);
                for(int j = 3 ; j >= 0 ; --j) {
                    if(temp[3 - j] != 0) {
                        border[i][j] = temp[3 - j];
                    }else {
                        border[i][j] = 0;
                    }
                }
            }
            break;
        }
        //重新将temp数组初始化为0
        memset(temp, 0, 4 * sizeof(int));
    }



    //随机生成一个2
    random(1, 2);

    //重新绘制
    update();
}

/**
 * 判断是否能够移动
 * @brief GameWidget::canMove
 * @return
 */
bool GameWidget::canMove()
{
    int temp[4];
    memset(temp, 0, 4 * sizeof(int));
    //获取每一行的数值
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            if(border[i][j] == 0) {
                return true;
            }
            temp[j] = border[i][j];
        }
        if(merge(temp, 4) > 0) {
            return true;
        }
    }
    //获取每一列
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            if(border[j][i] == 0) {
                return true;
            }
            temp[j] = border[j][i];
        }
        if(merge(temp, 4) > 0) {
            return true;
        }
    }
    return false;
}

int GameWidget::random()
{

}

/**
 * 合并数组，该数组元素为矩阵中某一行或某一列中所有不为0的元素
 * @brief GameWidget::merge
 * @param array
 * @param n
 * @return
 */
int GameWidget::merge(int *array, int n)
{
    int score = 0;
    int curIndex = 0;
    for(int j = curIndex + 1 ; j < n ; ) {

        if(curIndex == j) {
            //保证j总是在curIndex之前
            j++;
        }

        if(curIndex < n && j < n) {
            //如果当前元素不为0 且下一个元素等于当前元素，则它们能够合并
            if(array[curIndex] != 0 && array[j] == array[curIndex]) {
                //仅合并一次
                array[curIndex++] *= 2;
                array[j] = 0;
                //记录分数
                score += array[curIndex - 1];
                j++;
            }else if(array[curIndex] == 0 && array[j] != 0){
                //如果当前元素为0，下一个元素不为0，将他们交换，当前元素位置保持不变
                array[curIndex] = array[j];
                array[j] = 0;
                j++;
            }else {
                //其他情况，无法合并且下一个元素无法向前，当前指针前移，下一个位置保持不动
                curIndex++;
            }
        }else {
            break;
        }
    }
    return score;
}

void GameWidget::onScoreIncr(int incre)
{

}

void GameWidget::onGameOver()
{

}

void GameWidget::onWin()
{

}


/**
 * 处理键盘事件 up down left right
 * @brief GameWidget::keyPressEvent
 * @param event
 */
void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        event->accept();
        switch (event->key()) {
        case Qt::Key_Up:
            move(Direct::Up);
            break;
        case Qt::Key_Down:
            move(Direct::Down);
            break;
        case Qt::Key_Left:
            move(Direct::Left);
            break;
        case Qt::Key_Right:
            move(Direct::Right);
            break;
        }
    }

}

