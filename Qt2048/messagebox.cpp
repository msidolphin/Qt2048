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
    this->initComponent();
    this->initLayout();
    this->initConnection();

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //以模态框的形式展示
    this->setWindowModality(Qt::WindowModal);
}

void MessageBox::setMessage(const QString &text)
{
    this->mMessageLabel->setText(text);
}

void MessageBox::setModel(const Model & model)
{
    if(model == Model::SUCCESS) {
        this->mResetButton->setVisible(true);
        this->mContinueButton->setVisible(true);
    }else {
        //隐藏继续按钮
        this->mContinueButton->setVisible(false);
    }
}

void MessageBox::initComponent()
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

}

void MessageBox::initLayout()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(this->mResetButton);
    hLayout->addWidget(this->mContinueButton);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(this->mMessageLabel, 0, Qt::AlignCenter);
    vLayout->addLayout(hLayout);
    vLayout->setSpacing(10);
    this->setLayout(vLayout);

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
    QBitmap bitMap(QSize(this->width(), this->height()));
    bitMap.fill();
    QPainter p(&bitMap);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bitMap.rect(), 20, 20);
    setMask(bitMap);

    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#FAF8F0"));
    setPalette(pal);
    return QDialog::paintEvent(event);
}
