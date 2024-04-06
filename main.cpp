#include "mainwindow.h"

#include <QApplication>
#include <QLockFile>

#define LOCKFILENAME "LostArkNoteLock"

int main(int argc, char* argv[])
{
    QCoreApplication::setApplicationName("LostArkNote");
    QApplication a(argc, argv);
    MainWindow w;

    const QString _default_config_dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir _dir(_default_config_dir);


    //程序锁，仅允许打开一个程序
    QLockFile _lockapp(_dir.absoluteFilePath(LOCKFILENAME));
    if(!_lockapp.tryLock())
    {
        qDebug() << "程序已打开";
        return 0;
    }


    {
        //记忆上次打开位置
        QSettings _app_setting(_dir.absoluteFilePath(DEFAULTAPPCONFIGNAME), QSettings::IniFormat);
        int _height = _app_setting.value("WindowGeometry/Height").toInt();
        int _width = _app_setting.value("WindowGeometry/Width").toInt();
        int _x = _app_setting.value("WindowGeometry/X").toInt();
        int _y = _app_setting.value("WindowGeometry/Y").toInt();
        if(_height != 0 && _width != 0)
            w.setGeometry(_x, _y, _width, _height);

        //配置文件更新地址
        QString _updateUrl = _app_setting.value("UpdateData/UpdateURL").toString();
        if(_updateUrl.isEmpty())
        {
            _app_setting.setValue("UpdateData/UpdateURL", DEFAULTUPDATECONFIGURL);
            _updateUrl = DEFAULTUPDATECONFIGURL;
        }
        w.SetUpdateUrl(_updateUrl);

        //默认配置保存目录
        QString _config_dir = _app_setting.value("DataConfig/RootPath").toString();
        w.SetDefaultConfigDir(_config_dir.isEmpty() ? _default_config_dir : _config_dir);
    }

    w.SetFinished();
    w.show();

    return a.exec();
}
