#include "charactersetting.h"
#include "ui_charactersetting.h"

CharacterSetting::CharacterSetting(QWidget *parent, QString character_nick_name, double character_score) :
    QDialog(parent),
    ui(new Ui::CharacterSetting)
{
    ui->setupUi(this);
    ui->m_lineedit_character_nickname->setText(character_nick_name);
    ui->m_doubleSpinBox_character_score->setValue(character_score);
}

CharacterSetting::~CharacterSetting()
{
    delete ui;
}

QString CharacterSetting::GetCharacterNickName()
{
    return ui->m_lineedit_character_nickname->text();
}

double CharacterSetting::GetCharacterScore()
{
    return ui->m_doubleSpinBox_character_score->value();
}

