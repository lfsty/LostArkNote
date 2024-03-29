#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "character.h"
#include "todosinglewidget.h"
#include "INFO.h"
#include <QVector>
#include <QMap>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QAbstractButton>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QVector<Character*> m_vect_character;
    QVector<ToDoSingleStruct> m_AllToDoList;
private:
    void read_AllToDoList(const QString& file_path);
    void read_ExistData(const QString& file_path);
    void clear_ToDoList();
private slots:
    void on_action_triggered();
    void closeEvent(QCloseEvent* event);
    void OnUpdateToDoListUI(const int& index_character, const QVector<ToDoSingleStruct>& todolist_str);
    void OnUpdateToDoListFinished(const int& index_character, const ToDoSingleStruct& todo_list_struct);
    void AddNewCharacter(Character* new_character);
    void on_action_3_triggered();

    void on_action_4_triggered();

private:
    //文件存储相关
    QString m_savefile_dir_path;
private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
