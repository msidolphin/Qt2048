#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#define DEBUG

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class MessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);
    void setMessage(const QString & text);
private:
    QLabel *mMessageLabel;
    QPushButton *mResetButton;
    QPushButton *mContinueButton;
private:
    void initConnection();
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void resetBtnClicked();
    void continueBtnClicked();
private slots:
    void onContinue();
    void onReset();
};

#endif // BASEWINDOW_H
