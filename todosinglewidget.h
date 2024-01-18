#ifndef TODOSINGLEWIDGET_H
#define TODOSINGLEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "INFO.h"
namespace Ui
{
    class ToDoSingleWidget;
}

class ToDoSingleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToDoSingleWidget(const int &index_character, const ToDoSingleStruct &todo_single_struct, QWidget *parent = nullptr);
    ~ToDoSingleWidget();

private slots:
    void on_m_checkBox_status_toggled(bool checked);
    void mousePressEvent(QMouseEvent *event);
private:
    int m_index_character;
    ToDoSingleStruct m_todo_single_struct;
private:
    Ui::ToDoSingleWidget *ui;
signals:
    void sig_finish_status_changed(const int &index_character, const ToDoSingleStruct &todo_list_struct);
};

#endif // TODOSINGLEWIDGET_H
