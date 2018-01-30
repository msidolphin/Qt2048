#include "gamewindow.h"
#include <QFont>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{
    int score = 0;

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);

    mScoreLabel = new QLabel(QString("Score:\n%1").arg(score), this);
    mScoreLabel->setGeometry(209, 20, 180, 70);
    mScoreLabel->setFont(font);
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setStyleSheet("QLabel {color: orange;background: #FFFFCC;border: %1px solid orange;border-radius: %2px;}");
}

GameWindow::~GameWindow()
{

}
