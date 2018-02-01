#include "basewindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPainterPath>

BaseWindow::BaseWindow(QWidget *parent)
    : QWidget(parent)
{
    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlag(Qt::FramelessWindowHint);

    //设置窗口背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    initTitleBar();
}

void BaseWindow::initTitleBar()
{
    this->titleBar = new CustomTitleBar(this);
    this->titleBar->move(0, 0);

    this->titleBar->setTitleBarContent(QString::fromStdString("2048"));
    this->titleBar->setButtonType(ButtonType::MIN_MAX_CLOSE);
    this->titleBar->setWidth(this->width());

    connect(this->titleBar, SIGNAL(minButtonClicked()),     this, SLOT(onMinButtonClicked()));
    connect(this->titleBar, SIGNAL(maxButtonClicked()),     this, SLOT(onMaxButtonClicked()));
    connect(this->titleBar, SIGNAL(restoreButtonClicked()), this, SLOT(onRestoreButtonClicked()));
    connect(this->titleBar, SIGNAL(closeButtonClicked()),   this, SLOT(onCloseButtonClicked()));
}

void BaseWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(238, 223, 204)));

    return QWidget::paintEvent(event);
}

void BaseWindow::onMinButtonClicked()
{
    this->showMinimized();
}

void BaseWindow::onMaxButtonClicked()
{
    this->titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect factRect    = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(factRect);
}

void BaseWindow::onRestoreButtonClicked()
{
    QPoint windowPos;
    QSize  windowSize;
    this->titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::onCloseButtonClicked()
{
    this->close();
}
