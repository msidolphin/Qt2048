#include <QApplication>
#include "gamewindow.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow game;
    game.show();
    return a.exec();
}
