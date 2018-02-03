#include "gamewindow.h"
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <QCursor>
#include <QPalette>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "qfilehelper.h"

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{
    initComponent();
    initConnection();
    initLayout();
    this->setWindowTitle("2048");
    this->setFixedSize(QSize(420, 550));
}

void GameWindow::initComponent()
{
    //设置背景色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(20);

    //2048大标题字体
    QFont titleFont;
    titleFont.setFamily("Consolas");
    titleFont.setBold(true);
    titleFont.setPixelSize(65);

    mTitleLabel = new QLabel("2048");
    mTitleLabel->setFont(titleFont);

    mTitleLabel->setAlignment(Qt::AlignLeft);
    mTitleLabel->setStyleSheet(QString("QLabel {color: #756d65; border: 0px solid orange;border-radius: 0px;}"));

    mResetBtn = new QPushButton("NEW GAME");
    mResetBtn->setFixedWidth(150);
    mResetBtn->setFixedHeight(40);
    mResetBtn->setFont(font);
    mResetBtn->setCursor(QCursor(Qt::PointingHandCursor));
    mResetBtn->setStyleSheet(QString("QPushButton {color: #fff;background: #917a63;border: %1px solid;border-radius: %2px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}").arg(0).arg(5));
    mResetBtn->setDisabled(false);
    connect(mResetBtn, SIGNAL(clicked(bool)), this, SLOT(onGameReset()));

    gameWidget = new GameWidget;

    mScoreLabel = new QLabel(QString("Score:\n%1").arg(gameWidget->score));
    mScoreLabel->setFont(font);
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setStyleSheet(QString("QLabel {color: #fff;background: #bfae9e;border: %1px solid;border-radius: %2px;}").arg(0).arg(5));

    mHighScoreLabel = new QLabel(QString("High Score:\n%1").arg(gameWidget->highScore));
    mHighScoreLabel->setFixedWidth(140);
    mHighScoreLabel->setFont(font);
    mHighScoreLabel->setAlignment(Qt::AlignCenter);
    mHighScoreLabel->setStyleSheet(QString("QLabel {color: #fff;background: #bfae9e;border: %1px solid;border-radius: %2px;}").arg(0).arg(5));

    this->messageBox = new MessageBox(this);
    this->messageBox->hide();
}

/**
 * 初始化连接
 * @brief GameWindow::initConnection
 */
void GameWindow::initConnection()
{

    /**
     * GameWidget
     * scoreIncre(int)      分数新增信号
     * gameOver(bool)       游戏结束信号
     * highScoreUpdate(int) 更新最高分数信号
     */
    connect(gameWidget, SIGNAL(scoreIncre(int)), this, SLOT(onScoreIncre(int)));
    connect(gameWidget, SIGNAL(gameOver(bool)),  this, SLOT(onGameOver(bool)));
    connect(gameWidget, SIGNAL(highScoreUpdate(int)), this, SLOT(onHighScoreUpdate(int)));

    /**
     * GameWindow
     * saveHighScore() 保存最高分数信号
     */
    connect(this, SIGNAL(saveHighScore()), gameWidget, SLOT(onClosed()));

    /*
     * 消息对话框
     * continueBtnClicked() 继续按钮被按下信号
     * resetBtnClicked()    重置按钮被按下信号
     */
    connect(this->messageBox, SIGNAL(continueBtnClicked()), this, SLOT(onContinue()));
    connect(this->messageBox, SIGNAL(resetBtnClicked()), this, SLOT(onReset()));
}

/**
 * 初始化布局
 * @brief GameWindow::initLayout
 */
void GameWindow::initLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(this->mTitleLabel);
    topLayout->addWidget(this->mScoreLabel);
    topLayout->addWidget(this->mHighScoreLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(this->mResetBtn, 0, Qt::AlignRight);
    mainLayout->addWidget(this->gameWidget);

    this->setLayout(mainLayout);
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

void GameWindow::closeEvent(QCloseEvent *event)
{
    emit saveHighScore();
    return QWidget::closeEvent(event);
}


void GameWindow::onScoreIncre(int score)
{
    mScoreLabel->setText(QString("Score:\n%1").arg(score));
}

void GameWindow::onHighScoreUpdate(int score)
{
    this->mHighScoreLabel->setText(QString("High Score:\n%1").arg(score));
}

void GameWindow::onGameOver(bool isWin)
{
    if(isWin) {
        this->messageBox->setMessage("VICTORY!");
        this->messageBox->setModel(Model::SUCCESS);
        this->messageBox->show();

    }else {
        this->messageBox->setMessage("GAME OVER");
        this->messageBox->setModel(Model::FAILED);
        this->messageBox->show();
    }
}

void GameWindow::onGameReset()
{
    emit saveHighScore();
    gameWidget->reset();
}

void GameWindow::onContinue()
{
    this->gameWidget->continuePlay();
    this->messageBox->hide();
}

void GameWindow::onReset()
{
    emit saveHighScore();
    this->messageBox->hide();
    this->gameWidget->reset();
}
