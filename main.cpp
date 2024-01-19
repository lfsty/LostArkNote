#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setApplicationName("LostArkNote");
    QApplication a(argc, argv);
    MainWindow w;

    {
        //记忆上次打开位置
        QDir _dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
        QSettings _app_setting(_dir.absoluteFilePath(DEFAULTAPPCONFIGNAME), QSettings::IniFormat);
        int _height = _app_setting.value("WindowGeometry/Height").toInt();
        int _width = _app_setting.value("WindowGeometry/Width").toInt();
        int _x = _app_setting.value("WindowGeometry/X").toInt();
        int _y = _app_setting.value("WindowGeometry/Y").toInt();
        if(_height != 0 && _width != 0)
        {
            w.setGeometry(_x, _y, _width, _height);
        }
    }

    w.show();
    return a.exec();
}
