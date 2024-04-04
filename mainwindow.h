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
#include "downloadfile.h"
#include "BASEFUNCTION.h"
#include "overview.h"

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
    void read_finished_data(const QString& file_path);
    void clear_ToDoList();
    void update_Config(const QByteArray& data);
private slots:

    void closeEvent(QCloseEvent* event);
    void OnUpdateToDoListUI(const int& index_character, const QVector<ToDoSingleStruct>& todolist_str);
    void OnUpdateToDoListFinished(const int& index_character, const ToDoSingleStruct& todo_list_struct);
    void AddNewCharacter(Character* new_character);
    void on_open_config_dir_triggered();

    void on_clean_finished_status_triggered();

    void on_add_character_triggered();

    void on_update_config_triggered();

    void on_open_app_config_dir_triggered();

    void on_overview_triggered();

private:
    //文件存储相关
    QString m_savefile_dir_path;
    //副本相关文件保存地址
    QString m_config_path;
    //完成情况文件保存地址
    QString m_finished_config_path;
    QString m_update_url;
    DownloadFile* m_downloadFile = nullptr;
public:
    void SetUpdateUrl(const QString& url);
    void SetFinished();
    void SetDefaultConfigDir(const QString& dir_path);
private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
