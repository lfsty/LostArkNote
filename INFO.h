#ifndef INFO_H
#define INFO_H
#include <QString>

struct ToDoSingleStruct
{
    QString name;
    int low_limit;
    int high_limit;
    bool is_finished = false;
    bool is_show = false;
};

#endif // INFO_H
