#ifndef INFO_H
#define INFO_H
#include <QString>

#define DEFAULTCONFIGNAME "config.json"
#define DEFAULTFINISHEDDATANAME "data.json"
#define DEFAULTAPPCONFIGNAME "config.ini"

struct ToDoSingleStruct
{
    QString name;
    int low_limit;
    int high_limit;
    bool is_finished = false;
    bool is_show = false;
    bool is_twoWeek = false;
};

#endif // INFO_H
