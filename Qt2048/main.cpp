#include <QApplication>
#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow game;
    game.show();
    return a.exec();
}
