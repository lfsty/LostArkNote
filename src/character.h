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
                       double score = 0,
                       const QVector<ToDoSingleStruct>& exist_data = QVector<ToDoSingleStruct> {});
    ~Character();

private:
    Ui::Character* ui;

protected:
    void mousePressEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void OnCharacterNickNameChanged(const QString& nick_name);
    void OnCharacterScoreChanged(const double& score);
public:
    void UpdateToDoListUI();
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
    void updateStatus();
signals:
    void sig_character_score_changed(const double& score);
    void sig_character_nick_name_changed(const QString& nick_name);
    void sig_update_todo_list(const int& index_character, const QVector<ToDoSingleStruct>& todo_list);
    void sig_delete(const int& index);
    void sig_checked(const int& index);
};

#endif // CHARACTER_H
