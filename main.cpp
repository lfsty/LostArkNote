#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setApplicationName("LostArkNote");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
