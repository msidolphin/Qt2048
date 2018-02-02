#include "gamewindow.h"
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <QCursor>
#include <QPalette>
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{

    //设置背景色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(20);

    QFont titleFont;
    titleFont.setFamily("Consolas");
    titleFont.setBold(true);
    titleFont.setPixelSize(65);

    mTitleLabel = new QLabel("2048", this);
    mTitleLabel->setFont(titleFont);

    mTitleLabel->setAlignment(Qt::AlignLeft);
    mTitleLabel->setGeometry(20, 20, 300, 100);
    mTitleLabel->setStyleSheet(QString("QLabel {color: #756d65; border: 0px solid orange;border-radius: 0px;}"));

    mResetBtn = new QPushButton("NEW GAME", this);
    mResetBtn->setGeometry(300, 120, 150, 35);
    mResetBtn->setFont(font);
    mResetBtn->setCursor(QCursor(Qt::PointingHandCursor));
    mResetBtn->setStyleSheet(QString("QPushButton {color: #fff;background: #917a63;border: %1px solid;border-radius: %2px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}").arg(0).arg(5));
    mResetBtn->setDisabled(false);
    connect(mResetBtn, SIGNAL(clicked(bool)), this, SLOT(onGameReset()));

    gameWidget = new GameWidget(this);
    gameWidget->setGeometry((this->width() - 400) / 2 - 70, 200, 400, 400);

    /* 分数增加 和 游戏结束 true - 胜利 false - 失败 */
    connect(gameWidget, SIGNAL(scoreIncre(int)), this, SLOT(onScoreIncre(int)));
    connect(gameWidget, SIGNAL(gameOver(bool)),  this, SLOT(onGameOver(bool)));

    mScoreLabel = new QLabel(QString("Score:\n%1").arg(gameWidget->score), this);
    mScoreLabel->setGeometry(200, 20, 110, 70);
    mScoreLabel->setFont(font);
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setStyleSheet(QString("QLabel {color: #fff;background: #bfae9e;border: %1px solid;border-radius: %2px;}").arg(0).arg(5));

    mHighScoreLabel = new QLabel(QString("High Score:\n%1").arg(gameWidget->score), this);
    mHighScoreLabel->setGeometry(320, 20, 140, 70);
    mHighScoreLabel->setFont(font);
    mHighScoreLabel->setAlignment(Qt::AlignCenter);
    mHighScoreLabel->setStyleSheet(QString("QLabel {color: #fff;background: #bfae9e;border: %1px solid;border-radius: %2px;}").arg(0).arg(5));

    mResetBtn->installEventFilter(this);

    this->messageBox = new MessageBox(this);
    connect(this->messageBox, SIGNAL(continueBtnClicked()), this, SLOT(onContinue()));
    connect(this->messageBox, SIGNAL(resetBtnClicked()), this, SLOT(onReset()));
    this->messageBox->hide();

    this->setWindowTitle("2048");
    this->setFixedSize(500, 640);
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
        case Qt::Key_W:
            gameWidget->move(Direct::Up);
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            gameWidget->move(Direct::Down);
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            gameWidget->move(Direct::Left);
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            gameWidget->move(Direct::Right);
            break;
        }
    }

}


void GameWindow::onScoreIncre(int score)
{
    mScoreLabel->setText(QString("Score:\n%1").arg(score));
}

void GameWindow::onGameOver(bool isWin)
{
    if(isWin) {
        this->messageBox->setMessage("VICTORY!");
        this->messageBox->show();

    }else {
        this->messageBox->setMessage("GAME OVER");
        this->messageBox->show();
    }
}

void GameWindow::onGameReset()
{
    gameWidget->reset();
}

void GameWindow::onContinue()
{
    this->messageBox->hide();
}

void GameWindow::onReset()
{
    this->messageBox->hide();
    this->gameWidget->reset();
}
