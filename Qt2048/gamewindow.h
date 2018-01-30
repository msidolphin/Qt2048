#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QObject>
#include <QEvent>
#include "gamewidget.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void keyPressEvent(QKeyEvent *event);
private:
    //重新开始按钮
    QPushButton *mResetBtn;
    //关闭游戏按钮
    QPushButton *mCloseBtn;
    //标题
    QLabel *mTitleLabel;
    //分数面板
    QLabel *mScoreLabel;
    //分数面板
    QLabel *mHighScoreLabel;

    GameWidget *gameWidget;

    int score;
private slots:
    //分数改变
    void onScoreIncre(int score);
    //游戏结束(失败)
    void onGameOver();
    //游戏胜利
    void onWin();
};

#endif // GAMEWINDOW_H
