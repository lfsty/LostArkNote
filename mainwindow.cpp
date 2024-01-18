#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);

    m_savefile_dir_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir _dir(m_savefile_dir_path);
    if(!_dir.exists(m_savefile_dir_path))
    {
        _dir.mkdir(m_savefile_dir_path);
    }

    QString _config_file_path = _dir.absoluteFilePath("config.json");
    QFileInfo _fileInfo(_config_file_path);
    if(!_fileInfo.isFile())
    {
        QFile::copy(":/doc/DefaultConfig.json", _config_file_path);
    }

    read_AllToDoList(_config_file_path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_AllToDoList(const QString &file_path)
{
    QFile _loadFile(file_path);

    if(!_loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败" << file_path;
        return;
    }
    QByteArray _allData = _loadFile.readAll();
    _loadFile.close();

    QJsonParseError _jsonError;
    QJsonDocument _jsonDoc(QJsonDocument::fromJson(_allData, &_jsonError));
    if(_jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << _jsonError.errorString();
        return;
    }

    QJsonArray _rootList = _jsonDoc.array();
    for(int i = 0; i < _rootList.size(); i++)
    {
        QJsonObject _single_obj = _rootList[i].toObject();

        ToDoSingleStruct _todolist_struct;
        _todolist_struct.name = _single_obj["name"].toString();
        _todolist_struct.low_limit = _single_obj["limit"].toArray()[0].toInt();
        _todolist_struct.high_limit = _single_obj["limit"].toArray()[1].toInt();
        m_AllToDoList.append(_todolist_struct);
    }
}


void MainWindow::on_action_triggered()
{
    Character *_new_character = new Character(m_vect_character.size(), m_AllToDoList, this);
    m_vect_character.push_back(_new_character);

    connect(_new_character, &Character::sig_update_todo_list, this, &MainWindow::OnUpdateToDoListUI);

    ui->m_verticalLayout_character->insertWidget(ui->m_verticalLayout_character->indexOf(ui->m_verticalSpacer_character), _new_character);

    _new_character->UpdateToDoListUI();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QJsonArray _allCharacter_status_obj;
    for(auto item : m_vect_character)
    {
        _allCharacter_status_obj.append(item->GenSaveFile());
    }
    qDebug() << _allCharacter_status_obj;
}

void MainWindow::OnUpdateToDoListFinished(const int &index_character, const ToDoSingleStruct &todo_list_struct)
{
    m_vect_character[index_character]->UpdateToDoListFinished(todo_list_struct);
}

void MainWindow::OnUpdateToDoListUI(const int &index_character, const QVector<ToDoSingleStruct> &todolist_str)
{
    for(int i = 0; i < ui->m_verticalLayout_toDo->count(); i++)
    {
        ToDoSingleWidget *_todo_single_widget = qobject_cast<ToDoSingleWidget *>(ui->m_verticalLayout_toDo->itemAt(i)->widget());
        if(_todo_single_widget != 0)
        {
            _todo_single_widget->deleteLater();
        }
    }

    for(auto todo_struct : todolist_str)
    {
        if(todo_struct.is_show == true)
        {
            ToDoSingleWidget *_new_todo = new ToDoSingleWidget(index_character, todo_struct, this);
            connect(_new_todo, &ToDoSingleWidget::sig_finish_status_changed, this, &MainWindow::OnUpdateToDoListFinished);
            ui->m_verticalLayout_toDo->insertWidget(ui->m_verticalLayout_toDo->indexOf(ui->m_verticalSpacer_toDo), _new_todo);
        }
    }
}
