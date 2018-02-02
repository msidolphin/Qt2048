#include "messagebox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QBitmap>
#include <QPalette>
#include <QPainter>

MessageBox::MessageBox(QWidget *parent) : QDialog(parent)
{

    QFont messageFont;
    messageFont.setFamily("Consolas");
    messageFont.setBold(true);
    messageFont.setPixelSize(30);


    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(20);

    this->mMessageLabel = new QLabel;
    this->mResetButton  = new QPushButton("NEW GAME");
    this->mContinueButton  = new QPushButton("CONTINUE");

    this->mMessageLabel->setFont(messageFont);

    this->mResetButton->setFont(font);
    this->mContinueButton->setFont(font);

    this->mResetButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->mContinueButton->setCursor(QCursor(Qt::PointingHandCursor));

    this->setStyleSheet(QString("QWidget{border-radius: 20px;} QPushButton {color: #fff;background: #917a63;border: %1px solid;border-radius: %2px; padding:15px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}").arg(0).arg(5));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(this->mResetButton);
    hLayout->addWidget(this->mContinueButton);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(this->mMessageLabel, 0, Qt::AlignCenter);
    vLayout->addLayout(hLayout);

    this->setLayout(vLayout);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //以模态框的形式展示
    this->setWindowModality(Qt::WindowModal);

    initConnection();
}

void MessageBox::setMessage(const QString &text)
{
    this->mMessageLabel->setText(text);
}

void MessageBox::initConnection()
{
    connect(this->mContinueButton,  SIGNAL(clicked(bool)), this, SLOT(onContinue()));
    connect(this->mResetButton,     SIGNAL(clicked(bool)), this, SLOT(onReset()));
}


void MessageBox::onContinue()
{
    emit continueBtnClicked();
}

void MessageBox::onReset()
{
    emit resetBtnClicked();
}

void MessageBox::paintEvent(QPaintEvent *event) {
    QBitmap bitMao(this->size());
    bitMao.fill();
    QPainter p(&bitMao);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bitMao.rect(), 20, 20);
    setMask(bitMao);

    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);
    return QDialog::paintEvent(event);
}
