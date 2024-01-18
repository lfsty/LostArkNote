#include "todosinglewidget.h"
#include "ui_todosinglewidget.h"

ToDoSingleWidget::ToDoSingleWidget(const int& index_character, const ToDoSingleStruct& todo_single_struct, QWidget* parent) :
    QWidget(parent),
    m_index_character(index_character),
    m_todo_single_struct(todo_single_struct),
    ui(new Ui::ToDoSingleWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    ui->m_checkBox_status->setText(todo_single_struct.name);
    updateStatus(todo_single_struct.is_finished);
}

ToDoSingleWidget::~ToDoSingleWidget()
{
    delete ui;
}

void ToDoSingleWidget::on_m_checkBox_status_toggled(bool checked)
{
    m_todo_single_struct.is_finished = checked;
    emit sig_finish_status_changed(m_index_character, m_todo_single_struct);
    updateStatus(checked);
}

void ToDoSingleWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        ui->m_checkBox_status->setChecked(!ui->m_checkBox_status->isChecked());
    }
}

void ToDoSingleWidget::updateStatus(bool checked)
{
    if(checked == true)
    {
        ui->m_checkBox_status->setChecked(true);
        ui->m_checkBox_status->setStyleSheet("color:green;");
    }
    else
    {
        ui->m_checkBox_status->setChecked(false);
        ui->m_checkBox_status->setStyleSheet("color:red;");
    }
}

