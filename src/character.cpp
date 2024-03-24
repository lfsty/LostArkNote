#include "character.h"
#include "ui_character.h"
#include <QDebug>
Character::Character(const int& index_character, const QVector<ToDoSingleStruct>& AllToDoList, QWidget* parent, QString nick_name, double score, const QVector<ToDoSingleStruct>& exist_data) :
    QWidget(parent),
    ui(new Ui::Character),
    m_character_nick_name(nick_name),
    m_character_score(score),
    m_AllToDoList(AllToDoList),
    m_index_character(index_character)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    connect(this, &Character::sig_character_nick_name_changed, this, &Character::OnCharacterNickNameChanged);
    connect(this, &Character::sig_character_score_changed, this, &Character::OnCharacterScoreChanged);

    {
        //右键菜单
        QAction* _action_setting = new QAction("设置", this);
        connect(_action_setting, &QAction::triggered, this, [ = ]()
        {
            CharacterSetting _character_setting_dlg(parent, m_character_nick_name, m_character_score);
            if(_character_setting_dlg.exec() == QDialog::Accepted)
            {
                QString new_character_nick_name = _character_setting_dlg.GetCharacterNickName();
                double new_character_score = _character_setting_dlg.GetCharacterScore();
                if(new_character_nick_name != m_character_nick_name)
                {
                    emit sig_character_nick_name_changed(new_character_nick_name);
                }
                if(new_character_score != m_character_score)
                {
                    emit sig_character_score_changed(new_character_score);
                }
            }
        });

        QAction* _action_delete = new QAction("删除", this);
        connect(_action_delete, &QAction::triggered, this, [ = ]()
        {
            emit sig_delete(m_index_character);
            this->deleteLater();
        });

        m_menu.addAction(_action_setting);
        m_menu.addAction(_action_delete);
    }

    for(auto item : exist_data)
    {
        for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
        {
            if(item.name == iter->name)
            {
                iter->is_finished = item.is_finished;
            }
        }
    }

    emit sig_character_nick_name_changed(nick_name);
    emit sig_character_score_changed(score);
}

Character::~Character()
{
    delete ui;
}

void Character::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        UpdateToDoListUI();
        emit sig_checked(m_index_character);
    }
}

void Character::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu.exec(event->globalPos());
}

void Character::OnCharacterNickNameChanged(const QString& nick_name)
{
    m_character_nick_name = nick_name;
    ui->m_label_nickName->setText(nick_name);
}

void Character::OnCharacterScoreChanged(const double& score)
{
    m_character_score = score;
    ui->m_label_character_score->setText(QString::number(score));

    for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
    {
        if(m_character_score >= iter->low_limit && m_character_score < iter->high_limit)
        {
            iter->is_show = true;
        }
        else
        {
            iter->is_show = false;
        }
    }

    UpdateToDoListUI();
    updateStatus();
}

void Character::UpdateToDoListUI()
{
    emit sig_update_todo_list(m_index_character, m_AllToDoList);
}

void Character::UpdateToDoListFinished(const ToDoSingleStruct& single_struct)
{
    for(int i = 0; i < m_AllToDoList.size(); i++)
    {
        if(m_AllToDoList[i].name == single_struct.name)
        {
            m_AllToDoList[i].is_finished = single_struct.is_finished;
        }
    }
    updateStatus();
}

QJsonObject Character::GenSaveFile()
{
    QJsonObject _root_obj;
    _root_obj["name"] = m_character_nick_name;
    _root_obj["score"] = m_character_score;
    QJsonArray _finish_status;
    for(auto item : m_AllToDoList)
    {
        if(item.is_finished == true)
        {
            QJsonObject _finish_status_obj;
            _finish_status_obj["name"] = item.name;
            _finish_status_obj["isFinish"] = item.is_finished;
            _finish_status.append(_finish_status_obj);
        }
    }
    _root_obj["finishStatus"] = _finish_status;

    return _root_obj;
}

void Character::SetCharacterIndex(const int& index)
{
    m_index_character = index;
}

void Character::ClearAllFinished(bool is_two_week)
{
    for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
    {
        if(is_two_week == false)
        {
            //单周
            if(iter->is_twoWeek == false)
            {
                iter->is_finished = false;
            }
        }
        else
        {
            //双周
            iter->is_finished = false;
        }
    }
    updateStatus();
}

void Character::updateStatus()
{
    bool _status = true;
    int _remain_num = 0;
    for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
    {
        if(iter->is_show == true)
        {
            _status &= iter->is_finished;
            if(iter->is_finished == false)
                _remain_num++;
        }
    }

    ui->m_label_remain_num->setText(QString("剩余：") + QString::number(_remain_num));

    if(_status == true)
    {
        ui->m_label_character_score->setStyleSheet("color:green;");
        ui->m_label_nickName->setStyleSheet("color:green;");
        ui->m_label_remain_num->setStyleSheet("color:green;");
    }
    else
    {
        ui->m_label_character_score->setStyleSheet("color:red;");
        ui->m_label_nickName->setStyleSheet("color:red;");
        ui->m_label_remain_num->setStyleSheet("color:red;");
    }
}
