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
#include <cstdio>
#include <random>
#include <iostream>
#include "qfilehelper.h"

using namespace std;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    srand(time(nullptr));

    //分数初始化为0
    this->score = 0;
    this->isWin = false;
    this->isHighScoreUpdate = false;
    this->isMove = false;
    this->rx = this->ry = 20;

    loadHighScore();

    //设置背景色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);

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
    backgroundColor.insert(QString::number(512),    QColor::fromRgb(0xed, 0xc8, 0x50));
    backgroundColor.insert(QString::number(1024),   QColor::fromRgb(0xed, 0xc5, 0x3f));
    backgroundColor.insert(QString::number(2048),   QColor::fromRgb(0xed, 0xc2, 0x2e));

    initComponent();

}

GameWidget::~GameWidget()
{

}

void GameWidget::loadHighScore()
{
    this->mSettings = QFileHelper::openIniFile("high_score.ini");
    QVariant val = this->mSettings->value("highScore");
    if(val.isNull()) {
        this->mSettings->setValue("highScore", QVariant(0));
        this->highScore = 0;
    }else {
        this->highScore = val.toInt();
    }
    //发送信号
    emit highScoreUpdate(highScore);
}

void GameWidget::initComponent()
{
    initBoard();
    random(2);
    resize(400, 400);
    //先固定大小
    setFixedSize(this->width(), this->height());
}


/**
 * 绘制
 * @brief GameWidget::paintEvent
 * @param event
 */
void GameWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    //开启抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    //画笔设置为空，不绘制边框
    painter.setPen(Qt::NoPen);

    QBrush brush(QColor::fromRgb(141, 121, 81));
    painter.setBrush(brush);
    painter.drawRoundedRect(QRect(2, 2, width() - 4, height() - 4), this->rx, this->ry);

    w = (width() - 4) - 25;
    w /= 4;
    h = w;

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40);

    painter.setFont(font);

    for(int i = 0 ; i < ORDER ; ++i) {
        for(int j = 0 ; j < ORDER ; ++j) {
            //绘制背景
            brush.setColor(backgroundColor.value(QString::number(this->board[i][j])));
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), 20, 20);
            if(this->board[i][j] != 0) {
                //绘制文字
                painter.setPen(QColor::fromRgb(0, 0, 0));
                painter.drawText(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), Qt::AlignCenter, QString::number(this->board[i][j]));
                painter.setPen(Qt::NoPen);
            }
        }
    }
    return QWidget::paintEvent(event);
}

/**
 * 以8:1 的比例随机生成2和4
 * @brief GameWidget::random
 * @param count 随机生成个数
 */
void GameWidget::random(int count)
{
    bool hasZero = false;
    //判断是否存在0
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            if(this->board[i][j] == 0) {
                hasZero = true;
            }
        }
    }
    if(hasZero) {
        int col = -1;
        int row = -1;
        int r;
        while(count) {
            int colTmp = rand() % 4;
            int rowTmp = rand() % 4;
            if(colTmp != col && rowTmp != row) {
                col = colTmp;
                row = rowTmp;
                if(this->board[row][col] == 0) {
                    r = (rand() % 100) + 1; // 1 - 100
                    if(r > 0 && r <= 80) {
                        this->board[row][col] = 2;
                    }else if(r > 80 && r <= 100) {
                        this->board[row][col] = 4;
                    }
                    count --;
                }
            }
        }
    }
}

/**
 * 移动board矩阵，重写绘制
 * @brief GameWidget::move
 * @param direct
 */
void GameWidget::move(Direct direct)
{
        switch (direct){
        case Direct::Up:
            for(int i = 0 ; i < ORDER ; ++i) {
                int j = 0;
                for(int k = 0 ; k < ORDER ; ++k) {
                    while(k < ORDER && this->board[k][i] == 0) k++;
                    if(k >= ORDER) {
                        break;
                    }
                    if(k != j && this->board[j][i] == 0) {
                        this->isMove = true;
                        this->board[j][i] = this->board[k][i];
                        this->board[k][i] = 0;
                    }
                    if(j > 0 && this->board[j][i] == this->board[j-1][i] && !this->isMerge[j-1][i]) {
                        this->isMove = true;
                        this->board[j-1][i] *= 2;
                        this->board[j][i] = 0;
                        this->isMerge[j-1][i] = true;
                        score += this->board[j-1][i];
                        updateScore(this->board[j-1][i]);
                    }else {
                        j++;
                    }
                }
            }
            break;
        case Direct::Down:
            for(int i = 0 ; i < ORDER ; ++i) {
                int j = ORDER - 1;
                for(int k = ORDER - 1 ; k >= 0 ; --k) {
                    while(k >= 0 && this->board[k][i] == 0) k--;
                    if(k < 0) {
                        break;
                    }
                    if(k != j && this->board[j][i] == 0) {
                        this->isMove = true;
                        this->board[j][i] = this->board[k][i];
                        this->board[k][i] = 0;
                    }
                    if(j < ORDER - 1 && this->board[j][i] == this->board[j+1][i] && !this->isMerge[j+1][i]) {
                        this->isMove = true;
                        this->board[j+1][i] *= 2;
                        this->board[j][i] = 0;
                        this->isMerge[j+1][i] = true;
                        score += this->board[j+1][i];
                        updateScore(this->board[j+1][i]);
                    }else {
                        j--;
                    }
                }
            }
            break;
        case Direct::Left:
            for(int i = 0 ; i < ORDER ; ++i) {
                int j = 0;
                for(int k = 0 ; k < ORDER ; ++k) {
                    while(k < ORDER && this->board[i][k] == 0) k++;
                    if(k >= ORDER) {
                        break;
                    }
                    if(k != j && this->board[i][j] == 0) {
                        this->isMove = true;
                        this->board[i][j] = this->board[i][k];
                        this->board[i][k] = 0;
                    }
                    if(j > 0 && this->board[i][j] == this->board[i][j-1] && !this->isMerge[i][j-1]) {
                        this->isMove = true;
                        this->board[i][j-1] *= 2;
                        this->board[i][j] = 0;
                        this->isMerge[i][j-1] = true;
                        score += this->board[i][j-1];
                        updateScore(this->board[i][j-1]);
                    }else {
                        j++;
                    }
                }
            }
            break;
        case Direct::Right:
            for(int i = 0 ; i < ORDER ; ++i) {
                int j = ORDER - 1;
                for(int k = ORDER - 1 ; k >= 0 ; --k) {
                    while(k >= 0 && this->board[i][k] == 0) k--;
                    if(k < 0) {
                        break;
                    }
                    if(k != j && this->board[i][j] == 0) {
                        this->isMove = true;
                        this->board[i][j] = this->board[i][k];
                        this->board[i][k] = 0;
                    }
                    if(j < ORDER - 1 && this->board[i][j] == this->board[i][j+1] && !this->isMerge[i][j+1]) {
                        this->isMove = true;
                        this->board[i][j+1] *= 2;
                        this->board[i][j] = 0;
                        this->isMerge[i][j+1] = true;
                        score += this->board[i][j+1];
                        updateScore(this->board[i][j+1]);
                    }else {
                        j--;
                    }
                }
            }
            break;
        }    
    clearIsMerge();

    if(this->isWin) {
        return;
    }

    //判断是否移动过
    if(this->isMove) {
        this->isMove = false;
        random(1);
        //重新绘制
        update();
        //检查是否还能移动
        if(!canMove()) {
    #ifdef DEBUG
            qDebug() << "游戏失败";
    #endif
            emit gameOver(false);
        }
    }


}

void GameWidget::reset()
{
    //当前分数清零
    this->score = 0;
    //发送信号
    emit scoreIncre(score);
    this->isWin = false;
    this->isHighScoreUpdate = false;
    memset(board, 0, ORDER*ORDER*sizeof(int));
    random(2);
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
    for(int i = 0 ; i < ORDER ; ++i) {
        for(int j = 0 ; j < ORDER ; ++j) {
            if(this->board[i][j] == 0) {
                return true;
            }
            temp[j] = this->board[i][j];
        }
        if(merge(temp, 4) > 0) {
            return true;
        }
    }
    //获取每一列
    for(int i = 0 ; i < ORDER ; ++i) {
        for(int j = 0 ; j < ORDER ; ++j) {
            if(this->board[j][i] == 0) {
                return true;
            }
            temp[j] = this->board[j][i];
        }
        if(merge(temp, 4) > 0) {
            return true;
        }
    }
    return false;
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

void GameWidget::initBoard()
{
    for(int i = 0 ; i < ORDER ; ++i) {
        for(int j = 0 ; j < ORDER ; ++j) {
            this->board[i][j] = 0;
        }
    }
}

void GameWidget::clearIsMerge()
{
    for(int i = 0 ; i < ORDER ; ++i) {
        for(int j = 0 ; j < ORDER ; ++j) {
            this->isMerge[i][j] = false;
        }
    }
}

void GameWidget::updateScore(int mergeNumber)
{
    emit scoreIncre(this->score);
    if(this->score > this->highScore) {
        this->isHighScoreUpdate= true;
        this->highScore = this->score;
        emit highScoreUpdate(this->highScore);
    }
    if(mergeNumber == 2048 && !isWin) {
#ifdef DEBUG
    qDebug() << "游戏胜利";
#endif
        //胜利
        gameOver(true);
        isWin = true;
    }
}

void GameWidget::onClosed()
{
    if(this->isHighScoreUpdate) {
        this->mSettings->setValue("highScore", QVariant(this->highScore));
    }
}


