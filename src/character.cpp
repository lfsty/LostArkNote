#include "character.h"
#include "ui_character.h"
#include <QDebug>
Character::Character(const int& index_character, const QVector<ToDoSingleStruct>& AllToDoList, QWidget* parent, QString nick_name, double score) :
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
                    m_character_nick_name = new_character_nick_name;
                }
                if(new_character_score != m_character_score)
                {
                    m_character_score = new_character_score;
                }
                updateLeftStatusDisplay();
                emit sig_character_checked(m_index_character, m_AllToDoList);
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

    updateLeftStatusDisplay();
}

Character::~Character()
{
    delete ui;
}

void Character::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit sig_character_checked(m_index_character, m_AllToDoList);
    }
}

void Character::contextMenuEvent(QContextMenuEvent* event)
{
    m_menu.exec(event->globalPos());
}

void Character::UpdateAllToDoListVector(const QVector<ToDoSingleStruct>& AllToDoList)
{
    m_AllToDoList = CompareData_IsFinished(m_AllToDoList, AllToDoList);
    updateLeftStatusDisplay();
}

void Character::UpdateToDoListFinished(const ToDoSingleStruct& single_struct)
{
    for(int i = 0; i < m_AllToDoList.size(); i++)
    {
        if(m_AllToDoList[i].name == single_struct.name)
        {
            m_AllToDoList[i].is_finished = single_struct.is_finished;
            break;
        }
    }
    updateLeftStatusDisplay();
}

//生成完成信息的保存配置文件
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

//清空角色完成信息
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
    updateLeftStatusDisplay();
}

QString Character::GetCharaterNickName()
{
    return m_character_nick_name;
}

double Character::GetCharacterScore()
{
    return m_character_score;
}

const QVector<ToDoSingleStruct> Character::GetUnfinishedData()
{
    QVector<ToDoSingleStruct> _unfinished_data;

    for(const auto& item : m_AllToDoList){
        if(m_character_score >= item.low_limit && m_character_score < item.high_limit && item.is_finished == false)
        {
            _unfinished_data.push_back(item);
        }
    }

    return _unfinished_data;
}

//更新左侧的信息显示
void Character::updateLeftStatusDisplay()
{
    //更新左侧的UI 显示名称和装分
    ui->m_label_nickName->setText(m_character_nick_name);
    ui->m_label_character_score->setText(QString::number(m_character_score));

    //更新装分对应的副本
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

    // 计算剩余副本数
    bool _is_finished_status = true;
    int _remain_num = 0;
    for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
    {
        if(iter->is_show == true)
        {
            _is_finished_status &= iter->is_finished;
            if(iter->is_finished == false)
                _remain_num++;
        }
    }

    ui->m_label_remain_num->setText(QString("剩余：") + QString::number(_remain_num));

    //更新颜色
    if(_is_finished_status == true)
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
