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
#include "qfilehelper.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent),
    isHighScoreChanged(false),
    isWin(false),
    isFailed(false)
{
    srand(time(nullptr));

    //分数初始化为0
    score = 0;

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

    //初始化board为0
    memset(board, 0, sizeof(int) * 16);

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

    //开启抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    //画笔设置为空，不绘制边框
    painter.setPen(Qt::NoPen);

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

    int n = 4;

    for(int i = 0 ; i < n ; ++i) {
        for(int j = 0 ; j < n ; ++j) {
            //绘制背景
            brush.setColor(backgroundColor.value(QString::number(board[i][j])));
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), 20, 20);
            if(board[i][j] != 0) {
                //绘制文字
                painter.setPen(QColor::fromRgb(0, 0, 0));
                painter.drawText(QRectF(7 + (w + 5) * j, 7 + (h + 5)*i, w, h), Qt::AlignCenter, QString::number(board[i][j]));
                painter.setPen(Qt::NoPen);
            }
        }
    }
    return QWidget::paintEvent(event);
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
            if(board[i][j] == 0) {
                hasZero = true;
            }
        }
    }

    if(hasZero) {
        while(count) {
            i = rand() % 4;
            j = rand() % 4;
            if(board[i][j] == 0) {
                count--;
                board[i][j] = value;
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
                if(board[j][i] != 0) {
                    temp[index++] = board[j][i];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                score += merge(temp, num);
                for(int j = 0 ; j < 4 ; ++j) {
                    if(temp[j] != 0) {
                        board[j][i] = temp[j];
                    }else {
                        board[j][i] = 0;
                    }
                }
                break;
            }
            break;
        case Direct::Down:
            for(int j = 3 ; j >= 0 ; --j) {
                if(board[j][i] != 0) {
                    temp[index++] = board[j][i];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                score += merge(temp, num);
                for(int j = 3 ; j >= 0 ; --j) {
                    if(temp[3 - j] != 0) {
                        board[j][i] = temp[3 - j];
                    }else {
                        board[j][i] = 0;
                    }
                }
            }
            break;
        case Direct::Left:
            for(int j = 0 ; j < 4 ; ++j) {
                if(board[i][j] != 0) {
                    temp[index++] = board[i][j];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                score += merge(temp, num);
                //由于是向左，所以是以列的正向
                for(int j = 0 ; j < 4 ; ++j) {
                    if(temp[j] != 0) {
                        board[i][j] = temp[j];
                    }else {
                        board[i][j] = 0;
                    }
                }
            }
            break;
        case Direct::Right:
            for(int j = 3 ; j >= 0 ; --j) {
                if(board[i][j] != 0) {
                    temp[index++] = board[i][j];
                    num++;
                    flag = true;
                }
            }
            if(flag) {
                score += merge(temp, num);
                for(int j = 3 ; j >= 0 ; --j) {
                    if(temp[3 - j] != 0) {
                        board[i][j] = temp[3 - j];
                    }else {
                        board[i][j] = 0;
                    }
                }
            }
            break;
        }

        //发出分数更新信号
        emit scoreIncre(score);
        if(this->score > this->highScore) {
            this->isHighScoreChanged = true;
            this->highScore = this->score;
            emit highScoreUpdate(this->highScore);
        }

        if(score == 2048 && !isWin) {
#ifdef DEBUG
        qDebug() << "游戏胜利";
#endif
            //胜利
            gameOver(true);
            isWin = true;
            return;
        }

        //重新将temp数组初始化为0
        memset(temp, 0, 4 * sizeof(int));
    }

    //随机生成一个2
    random(1, 2);

    //重新绘制
    update();

    //检查是否还能移动
    if(!canMove() && !isFailed) {
#ifdef DEBUG
        qDebug() << "游戏失败";
#endif
        emit gameOver(false);
        isFailed = true;
    }
}

void GameWidget::reset()
{
    //当前分数清零
    score = 0;
    //发送信号
    emit scoreIncre(score);
    isWin = false;
    isFailed = false;
    memset(board, 0, ORDER*ORDER*sizeof(int));
    random(2, 4);
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
            if(board[i][j] == 0) {
                return true;
            }
            temp[j] = board[i][j];
        }
        if(merge(temp, 4) > 0) {
            return true;
        }
    }
    //获取每一列
    for(int i = 0 ; i < 4 ; ++i) {
        for(int j = 0 ; j < 4 ; ++j) {
            if(board[j][i] == 0) {
                return true;
            }
            temp[j] = board[j][i];
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

void GameWidget::onClosed()
{
    if(this->isHighScoreChanged) {
        this->mSettings->setValue("highScore", QVariant(this->highScore));
    }
}


