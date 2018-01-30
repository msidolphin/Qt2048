#include "gamewindow.h"
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>

using namespace std;

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);

    //重置按钮 TODO 自己实现 因为方向键影响聚焦

    mResetBtn = new QPushButton("Restart", this);
    mResetBtn->setGeometry(100, 120, 200, 50);
    mResetBtn->setFont(font);
    mResetBtn->setStyleSheet(QString("QPushButton {color: red;background: lightgray;border: %1px solid darkgray;border-radius: %2px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}").arg(3).arg(15));
    mResetBtn->setDisabled(true);
    mResetBtn->unsetCursor();



    gameWidget = new GameWidget(this);
    gameWidget->setGeometry(2, 200, 400, 400);
    //gameWidget->installEventFilter(this);
    connect(gameWidget, SIGNAL(scoreIncre(int)), this, SLOT(onScoreIncre(int)));


    mScoreLabel = new QLabel(QString("Score:\n%1").arg(gameWidget->score), this);
    mScoreLabel->setGeometry(209, 20, 180, 70);
    mScoreLabel->setFont(font);
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setStyleSheet(QString("QLabel {color: orange;background: #FFFFCC;border: %1px solid orange;border-radius: %2px;}").arg(5).arg(20));
}

GameWindow::~GameWindow()
{

}

/**
 * 处理键盘事件 up down left right
 * @brief GameWindow::keyPressEvent
 * @param event
 */
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        event->accept();
        switch (event->key()) {
        case Qt::Key_Up:
            gameWidget->move(Direct::Up);
            break;
        case Qt::Key_Down:
            gameWidget->move(Direct::Down);
            break;
        case Qt::Key_Left:
            gameWidget->move(Direct::Left);
            break;
        case Qt::Key_Right:
            gameWidget->move(Direct::Right);
            break;
        }
    }

}

void GameWindow::onScoreIncre(int score)
{
    mScoreLabel->setText(QString("Score:\n%1").arg(score));
}

void GameWindow::onGameOver()
{

}

void GameWindow::onWin()
{

}
