#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QCoreApplication>

namespace Ui
{
    class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    static About* GetInstance();
    ~About();
private:
    explicit About(QWidget* parent = nullptr);
private:
    Ui::About* ui;
};

#endif // ABOUT_H
