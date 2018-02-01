#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QWidget>
#include "customtitlebar.h"

class BaseWindow : public QWidget
{
    Q_OBJECT
public:
    BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
private:
    void initTitleBar();
    void paintEvent(QPaintEvent *event);
private slots:
    void onMinButtonClicked();
    void onMaxButtonClicked();
    void onRestoreButtonClicked();
    void onCloseButtonClicked();
protected:
    CustomTitleBar *titleBar;
};

#endif // BASEWINDOW_H
