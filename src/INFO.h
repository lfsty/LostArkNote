#ifndef INFO_H
#define INFO_H
#include <QString>
#include <QVector>

#define DEFAULTCONFIGNAME "config.json"
#define DEFAULTFINISHEDDATANAME "data.json"
#define DEFAULTAPPCONFIGNAME "config.ini"
#define DEFAULTUPDATECONFIGURL "https://raw.githubusercontent.com/lfsty/LostArkNote/main/doc/DefaultConfig.json"

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

#endif // INFO_H
