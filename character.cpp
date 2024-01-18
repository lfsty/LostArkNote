#include "character.h"
#include "ui_character.h"
#include <QDebug>
Character::Character(const int &index_character, const QVector<ToDoSingleStruct> &AllToDoList, QWidget *parent, QString nick_name, double score) :
    QWidget(parent),
    ui(new Ui::Character),
    m_character_nick_name(nick_name),
    m_character_score(score),
    m_AllToDoList(AllToDoList),
    m_index_character(index_character)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    connect(this, &Character::sig_character_nick_name_changed, this, &Character::OnCharacterNickNameChanged);
    connect(this, &Character::sig_character_score_changed, this, &Character::OnCharacterScoreChanged);

    {
        //右键菜单
        QAction *_action_setting = new QAction("设置", this);
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

        m_menu.addAction(_action_setting);
    }
}

Character::~Character()
{
    delete ui;
}

void Character::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        UpdateToDoListUI();
    }
}

void Character::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu.exec(event->globalPos());
}

void Character::OnCharacterNickNameChanged(const QString &nick_name)
{
    m_character_nick_name = nick_name;
    ui->m_label_nickName->setText(nick_name);
}

void Character::OnCharacterScoreChanged(const double &score)
{
    m_character_score = score;
    ui->m_label_character_score->setText(QString::number(score));
    UpdateToDoListUI();
}

void Character::UpdateToDoListUI()
{
    for(auto iter = m_AllToDoList.begin(); iter != m_AllToDoList.end(); iter++)
    {
        if(m_character_score >= iter->low_limit)
        {
            iter->is_show = true;
        }
        else
        {
            iter->is_show = false;
        }
    }
    emit sig_update_todo_list(m_index_character, m_AllToDoList);
}

void Character::UpdateToDoListFinished(const ToDoSingleStruct &single_struct)
{
    for(int i = 0; i < m_AllToDoList.size(); i++)
    {
        if(m_AllToDoList[i].name == single_struct.name)
        {
            m_AllToDoList[i].is_finished = single_struct.is_finished;
        }
    }
}

QJsonObject Character::GenSaveFile()
{
    QJsonObject _root_obj;
    _root_obj["name"] = m_character_nick_name;
    _root_obj["score"] = m_character_score;
    QJsonArray _finish_status;
    for(auto item : m_AllToDoList)
    {
        QJsonObject _finish_status_obj;
        _finish_status_obj["name"] = item.name;
        _finish_status_obj["isFinish"] = item.is_finished;
        _finish_status.append(_finish_status_obj);
    }

    return _root_obj;
}
