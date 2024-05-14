#ifndef INFO_H
#define INFO_H
#include <QString>
#include <QVector>

#define DEFAULTCONFIGNAME "config.json"
#define DEFAULTFINISHEDDATANAME "data.json"
#define DEFAULTAPPCONFIGNAME "config.ini"
#define DEFAULTUPDATECONFIGNAME "update.ini"
#define DEFAULTINCOMEIMGNAME "income.png"
#define DEFAULTUPDATECONFIGURL "https://raw.githubusercontent.com/lfsty/LostArkNote/main/doc/DefaultConfig.json"
#define DEFAULTUPDATEINCOMEURL "https://raw.githubusercontent.com/lfsty/LostArkNote/main/doc/income.png"
#define DEFAULTUPDATETIME "2024-04-10 06:00:00"
#define DEFAULEISTWOWEEK true
#define VERSION "0.2.8"

struct ToDoSingleStruct
{
    QString name;
    int low_limit;
    int high_limit;
    bool is_finished = false;
    bool is_show = false;
    bool is_twoWeek = false;
};

struct CharacterToDoListStruct
{
    QString nick_name;
    double character_score;
    QVector<ToDoSingleStruct> todo_struct_vect;
};

struct OnCloseData
{
    int width;
    int height;
    int x;
    int y;
};

#endif // INFO_H
