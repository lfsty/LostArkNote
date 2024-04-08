#include "mainwindow.h"

#include <QApplication>
#include <QLockFile>
#include <QDateTime>

#define LOCKFILENAME "LostArkNoteLock"

int main(int argc, char* argv[])
{
    QCoreApplication::setApplicationName("LostArkNote");
    QCoreApplication::setApplicationVersion("0.2.7");
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

    //记忆上次打开位置
    QSettings _app_setting(_dir.absoluteFilePath(DEFAULTAPPCONFIGNAME), QSettings::IniFormat);
    int _height = _app_setting.value("WindowGeometry/Height").toInt();
    int _width = _app_setting.value("WindowGeometry/Width").toInt();
    int _x = _app_setting.value("WindowGeometry/X").toInt();
    int _y = _app_setting.value("WindowGeometry/Y").toInt();
    if(_height != 0 && _width != 0)
        w.setGeometry(_x, _y, _width, _height);

    //配置文件更新地址
    QString _updateUrl;
    if(_app_setting.contains("UpdateData/UpdateURL"))
    {
        _updateUrl = _app_setting.value("UpdateData/UpdateURL").toString();
    }
    else
    {
        _app_setting.setValue("UpdateData/UpdateURL", DEFAULTUPDATECONFIGURL);
        _updateUrl = DEFAULTUPDATECONFIGURL;
    }
    w.SetUpdateUrl(_updateUrl);

    //默认配置保存目录
    QString _config_dir = _app_setting.value("DataConfig/RootPath").toString();
    w.SetDefaultConfigDir(_config_dir.isEmpty() ? _default_config_dir : _config_dir);

    //游戏更新周期
    QString _update_time;
    bool _is_two_week;
    bool _is_need_update = false;
    if(_app_setting.contains("UpdateData/UpdateTime"))
    {
        _update_time = _app_setting.value("UpdateData/UpdateTime").toString();
        _is_two_week = _app_setting.value("UpdateData/IsTwoWeek").toBool();
    }
    else
    {
        _update_time = DEFAULTUPDATETIME;
        _is_two_week = DEFAULEISTWOWEEK;
        _app_setting.setValue("UpdateData/UpdateTime", DEFAULTUPDATETIME);
        _app_setting.setValue("UpdateData/IsTwoWeek", DEFAULEISTWOWEEK);
    }
    QDateTime _current_date = QDateTime::currentDateTime();
    QDateTime _last_update_date = QDateTime::fromString(_update_time, "yyyy-MM-dd hh:mm:ss");
    qint64 _day_to_last_update = _last_update_date.daysTo(_current_date);
    if(_day_to_last_update < 0)
    {
        qDebug() << "系统时间出错";
    }
    else
    {
        qint64 _num_week = _day_to_last_update / 7;
        //如果超过一周了,更新时间
        if(_num_week != 0)
        {
            _last_update_date = _last_update_date.addDays(_num_week * 7);
            _is_two_week = _num_week % 2 == 0 ? _is_two_week : !_is_two_week;
            _app_setting.setValue("UpdateData/UpdateTime", _last_update_date.toString("yyyy-MM-dd hh:mm:ss"));
            _app_setting.setValue("UpdateData/IsTwoWeek", _is_two_week);
            _is_need_update = true;
            //如果超过两周不更新，则更新所有，真正的双周已经记录了
            if(_num_week >= 2)
            {
                _is_two_week = true;
            }
        }
    }

    w.SetFinished();
    w.show();

    if(_is_need_update)
        w.ClearAllFinished(_is_two_week);

    return a.exec();
}
