#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QLabel>
#include <QKeyEvent>
#include <QOpenGLWidget>

//键盘方向
enum Direct {
   Left,
   Right,
   Up,
   Down
};

class GameWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    //分数
    int    score;
public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();
    void paintEvent(QPaintEvent *event);
    void move(Direct direct);
private:
    QMap<QString, QColor> backgroundColor;

    //游戏面板
    int border[4][4];
    double w = 30, h = 30;
    double rx, ry;

private:
    void random(int count, int value);
    bool canMove();
    int  random();
    int merge(int * array, int n);
signals:
    void scoreIncre(int);
};

#endif // GAMEWIDGET_H
