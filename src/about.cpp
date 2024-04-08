#include "about.h"
#include "ui_about.h"

About::About(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->m_label_version->setText(QCoreApplication::applicationVersion());
}

About* About::GetInstance()
{
    static About* _instance = new About();
    return _instance;
}

About::~About()
{
    delete ui;
}
