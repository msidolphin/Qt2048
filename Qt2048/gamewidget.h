#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QLabel>
#include <QKeyEvent>

//键盘方向
enum Direct {
   Left,
   Right,
   Up,
   Down
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();
    //void paintEvent();
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    QMap<QString, QColor> backgroundColor;

    //游戏面板
    int border[4][4];
    double w = 30, h = 30;
    double rx, ry;
    //分数
    int    score;

private:
    void random(int count, int value);
    void move(Direct direct);
    bool canMove();
    int  random();
    int merge(int * array, int n);
private slots:
    //分数改变
    void onScoreIncr(int incre);
    //游戏结束(失败)
    void onGameOver();
    //游戏胜利
    void onWin();
};

#endif // GAMEWIDGET_H
