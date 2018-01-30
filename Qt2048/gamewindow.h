#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "gamewidget.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = 0);
    ~GameWindow();
private:
    //重新开始按钮
    QPushButton *mResetBtn;
    //关闭游戏按钮
    QPushButton *mCloseBtn;
    //标题
    QLabel *mTitleLabel;
    //分数面板
    QLabel *mScoreLabel;

    GameWidget *gameWidget;

    int score;
};

#endif // GAMEWINDOW_H
