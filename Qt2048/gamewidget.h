#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QLabel>
#include <QSettings>
#include <QKeyEvent>

//#define DEBUG
#define ORDER 4

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
    //分数
    int score;
    int highScore;
public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();
    void paintEvent(QPaintEvent *event);
    void move(Direct direct);
    void reset();
private:
    QMap<QString, QColor> backgroundColor;

    //游戏面板
    int board[4][4];
    double w = 30, h = 30;
    double rx, ry;

    //是否胜利
    bool isWin;
    //是否失败
    bool isFailed;

    //最大分数是否改变
    bool isHighScoreChanged;

    QSettings *mSettings;

private:
    void random(int count, int value);
    bool canMove();
    int  random();
    int merge(int * array, int n);
signals:
    void scoreIncre(int);
    void highScoreUpdate(int);
    void gameOver(bool);
public slots:
    //关闭时保存槽函数
    void onClosed();
};

#endif // GAMEWIDGET_H
