#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

//#define DEBUG

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QObject>
#include <QEvent>
#include "gamewidget.h"
#include "messagebox.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void keyPressEvent(QKeyEvent *event);
private:
    void initLayout();
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

    MessageBox *messageBox;

    int score;
private slots:
    /* 分数改变 */
    void onScoreIncre(int score);
    /* 游戏结束 */
    void onGameOver(bool isWin);
    /* 重置游戏 */
    void onGameReset();
    void onContinue();
    void onReset();
};

#endif // GAMEWINDOW_H
