#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QDialog>
#include "INFO.h"
#include <QMap>
namespace Ui {
class Overview;
}

class Overview : public QDialog
{
    Q_OBJECT

public:
    static Overview* GetInstance();
    ~Overview();

public:
    void SetStatusData();
    void SetUnfinishedData(const QVector<CharacterToDoListStruct>& unfinished_data);
private:
    explicit Overview(QWidget *parent = nullptr);

private:
    Ui::Overview *ui;
};

#endif // OVERVIEW_H
