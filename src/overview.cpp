#include "overview.h"
#include "ui_overview.h"

Overview::Overview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
}

Overview *Overview::GetInstance()
{
    static Overview* _instance = new Overview();
    return _instance;
}

Overview::~Overview()
{
    delete ui;
}

void Overview::SetUnfinishedData(const QVector<CharacterToDoListStruct> &unfinished_data)
{
    ui->m_textEdit_overview->clear();
    QMap<QString,int> _all_status;

    for(const auto& item : unfinished_data){
        ui->m_textEdit_overview->insertPlainText(item.nick_name + " ");
        ui->m_textEdit_overview->insertPlainText(QString::number(item.character_score));
        for(const auto& _item : item.todo_struct_vect){
            ui->m_textEdit_overview->insertPlainText("\n     " + _item.name);
            _all_status[_item.name]++;
        }
        ui->m_textEdit_overview->insertPlainText("\n");
    }

    ui->m_textEdit_overview->append("总计：");
    for(auto iter = _all_status.begin();iter != _all_status.end();iter++){
        ui->m_textEdit_overview->append(iter.key()+":  "+"<font color=\"#FF0000\">"+QString::number(iter.value())+"个"+"</font>");
    }
}
