#ifndef CHARACTERSETTING_H
#define CHARACTERSETTING_H

#include <QDialog>

namespace Ui
{
    class CharacterSetting;
}

class CharacterSetting : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterSetting(QWidget *parent = nullptr, QString character_nick_name = "DefaultName", double character_score = 0);
    ~CharacterSetting();

public:
    QString GetCharacterNickName();
    double GetCharacterScore();

private:
    Ui::CharacterSetting *ui;
};

#endif // CHARACTERSETTING_H
