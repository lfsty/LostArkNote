#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
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

    QString _config_file_path = _dir.absoluteFilePath(DEFAULTCONFIGNAME);
    QFileInfo _fileInfo(_config_file_path);
    if(!_fileInfo.isFile())
    {
        QFile::copy(":/doc/DefaultConfig.json", _config_file_path);
    }

    read_AllToDoList(_config_file_path);


    QString _existed_data_path = _dir.absoluteFilePath(DEFAULTFINISHEDDATANAME);
    _fileInfo.setFile(_existed_data_path);
    if(_fileInfo.isFile())
    {
        read_ExistData(_existed_data_path);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_AllToDoList(const QString& file_path)
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

void MainWindow::read_ExistData(const QString& file_path)
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
        QVector<ToDoSingleStruct> _exist_data_map;

        QString _character_name = _single_obj["name"].toString();
        double _character_score = _single_obj["score"].toDouble();
        QJsonArray _finish_status_array = _single_obj["finishStatus"].toArray();
        for(auto item : _finish_status_array)
        {
            QJsonObject _todo_single_obj = item.toObject();
            ToDoSingleStruct _todo_single_struct;
            _todo_single_struct.is_finished = _todo_single_obj["isFinish"].toBool();
            _todo_single_struct.name = _todo_single_obj["name"].toString();
            _exist_data_map.append(_todo_single_struct);
        }

        Character* _new_character = new Character(m_vect_character.size(), m_AllToDoList, this, _character_name, _character_score, _exist_data_map);
        AddNewCharacter(_new_character);
    }
}

void MainWindow::clear_ToDoList()
{
    for(int i = 0; i < ui->m_verticalLayout_toDo->count(); i++)
    {
        ToDoSingleWidget* _todo_single_widget = qobject_cast<ToDoSingleWidget*>(ui->m_verticalLayout_toDo->itemAt(i)->widget());
        if(_todo_single_widget != 0)
        {
            _todo_single_widget->deleteLater();
        }
    }
}


void MainWindow::on_action_triggered()
{
    Character* _new_character = new Character(m_vect_character.size(), m_AllToDoList, this);
    AddNewCharacter(_new_character);
    _new_character->UpdateToDoListUI();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QJsonArray _allCharacter_status_obj;
    for(auto item : m_vect_character)
    {
        _allCharacter_status_obj.append(item->GenSaveFile());
    }
    QJsonDocument _save_json(_allCharacter_status_obj);

    QDir _dir(m_savefile_dir_path);
    QFile _save_finish_data(_dir.absoluteFilePath(DEFAULTFINISHEDDATANAME));
    if(!_save_finish_data.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "保存文件打开出错";
    }
    else
    {
        _save_finish_data.write(_save_json.toJson());
        _save_finish_data.close();
    }


    QString _existed_ini_path = _dir.absoluteFilePath(DEFAULTAPPCONFIGNAME);
    QSettings _app_setting(_existed_ini_path, QSettings::IniFormat);
    _app_setting.setValue("WindowGeometry/Height", this->height());
    _app_setting.setValue("WindowGeometry/Width", this->width());
    _app_setting.setValue("WindowGeometry/X", this->x());
    _app_setting.setValue("WindowGeometry/Y", this->y());
}

void MainWindow::OnUpdateToDoListFinished(const int& index_character, const ToDoSingleStruct& todo_list_struct)
{
    m_vect_character[index_character]->UpdateToDoListFinished(todo_list_struct);
}

void MainWindow::AddNewCharacter(Character* new_character)
{
    m_vect_character.push_back(new_character);
    connect(new_character, &Character::sig_update_todo_list, this, &MainWindow::OnUpdateToDoListUI);
    ui->m_verticalLayout_character->insertWidget(ui->m_verticalLayout_character->indexOf(ui->m_verticalSpacer_character), new_character);
    connect(new_character, &Character::sig_delete, this, [ = ](const int& index)
    {
        m_vect_character.erase(m_vect_character.begin() + index);
        for(int i = index; i < m_vect_character.size(); i++)
        {
            m_vect_character[i]->SetCharacterIndex(i);
        }
        clear_ToDoList();
    });

    connect(new_character, &Character::sig_checked, this, [ = ](const int& index)
    {
        for(int i = 0; i < m_vect_character.size(); i++)
        {
            if(i != index)
            {
                m_vect_character[i]->setStyleSheet("");
            }
            else
            {
                m_vect_character[i]->setStyleSheet("background-color: rgb(215, 215, 215);border-radius:10px;");
            }
        }
    });
}

void MainWindow::OnUpdateToDoListUI(const int& index_character, const QVector<ToDoSingleStruct>& todolist_str)
{
    clear_ToDoList();

    for(auto todo_struct : todolist_str)
    {
        if(todo_struct.is_show == true)
        {
            ToDoSingleWidget* _new_todo = new ToDoSingleWidget(index_character, todo_struct, this);
            connect(_new_todo, &ToDoSingleWidget::sig_finish_status_changed, this, &MainWindow::OnUpdateToDoListFinished);
            ui->m_verticalLayout_toDo->insertWidget(ui->m_verticalLayout_toDo->indexOf(ui->m_verticalSpacer_toDo), _new_todo);
        }
    }
}

void MainWindow::on_action_3_triggered()
{
    QMessageBox::StandardButton _button  = QMessageBox::question(this, "请求确认", "确定到周三了嘛，要清空了哦~", QMessageBox::Yes | QMessageBox::No, QMessageBox::NoButton);
    if (_button == QMessageBox::StandardButton::Yes)
    {
        for(int i = 0; i < m_vect_character.size(); i++)
        {
            m_vect_character[i]->ClearAllFinished();
        }
    }
    clear_ToDoList();
}


void MainWindow::on_action_4_triggered()
{
//    qDebug() << m_savefile_dir_path;
    QDesktopServices::openUrl(QUrl("file:" + m_savefile_dir_path, QUrl::TolerantMode));
}

