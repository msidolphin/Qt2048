﻿#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QLabel>
#include <QSettings>
#include <QKeyEvent>
#include <QPointF>

//#define DEBUG
#define ORDER 4

enum Direct {
   Up    = 0x0,
   Down  = 0x1,
   Left  = 0x2,
   Right = 0x3
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    //分数
    int score;
    int highScore;
public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();
    void paintEvent(QPaintEvent *event);
    void move(Direct direct);
    void reset();
    void continuePlay();
private:
    //游戏面板
    int board[4][4];

    bool isMerge[4][4];

    double w = 30, h = 30;
    double rx, ry;

    //是否胜利
    bool isWin;

    //是否移动过
    bool isMove;

    //最大分数是否改变
    bool isHighScoreUpdate;

    //ini
    QSettings *mSettings;


private:
    void random(int count);
    bool canMove();
    int merge(int * array, int n);
    void clearIsMerge();
    void updateScore(int mergetNumber);
    void loadHighScore();
    void initComponent();
    QColor getBackgroundColor(int number);
signals:
    void scoreIncre(int);
    void highScoreUpdate(int);
    void gameOver(bool);
public slots:
    //关闭时保存槽函数
    void onClosed();
};

#endif // GAMEWIDGET_H
