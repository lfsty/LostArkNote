#ifndef CHARACTER_H
#define CHARACTER_H

#include <QWidget>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include "charactersetting.h"
#include "INFO.h"
#include <QJsonObject>
#include <QJsonArray>
#include "basefunction.h"

namespace Ui
{
    class Character;
}

class Character : public QWidget
{
    Q_OBJECT

public:
    explicit Character(const int& index_character,
                       const QVector<ToDoSingleStruct>& AllToDoList,
                       QWidget* parent = nullptr,
                       QString nick_name = "DefaultName",
                       double score = 0);
    ~Character();

private:
    Ui::Character* ui;

protected:
    void mousePressEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
public:
    void UpdateAllToDoListVector(const QVector<ToDoSingleStruct>& AllToDoList);

    void UpdateToDoListFinished(const ToDoSingleStruct& single_struct);
    QJsonObject GenSaveFile();
    void SetCharacterIndex(const int& index);
    void ClearAllFinished(bool is_two_week);
private:
    //UI相关
    QMenu m_menu;
private:
    QString m_character_nick_name;
    double m_character_score;
    QVector<ToDoSingleStruct> m_AllToDoList;
    int m_index_character;
private:
    void updateLeftStatusDisplay();
signals:
    void sig_character_checked(const int& index_character, const QVector<ToDoSingleStruct>& todo_list);
    void sig_delete(const int& index);
};

#endif // CHARACTER_H
