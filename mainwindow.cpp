#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);

    m_downloadFile = new DownloadFile(this);

    connect(m_downloadFile, &DownloadFile::downloadFinished, this, &MainWindow::update_Config);
}

MainWindow::~MainWindow()
{
    delete ui;
    m_downloadFile->deleteLater();
}

//读取副本配置信息
void MainWindow::read_AllToDoList(const QString& file_path)
{
    m_AllToDoList.clear();
    QJsonDocument _json_doc = GetJsonDocumentFromFile(file_path);

    if(_json_doc.isEmpty())
    {
        return;
    }

    QJsonArray _rootList = _json_doc.object()["data"].toArray();
    for(int i = 0; i < _rootList.size(); i++)
    {
        QJsonObject _single_obj = _rootList[i].toObject();

        ToDoSingleStruct _todolist_struct;
        _todolist_struct.name = _single_obj["name"].toString();
        _todolist_struct.low_limit = _single_obj["limit"].toArray()[0].toInt();
        _todolist_struct.high_limit = _single_obj["limit"].toArray()[1].toInt();
        _todolist_struct.is_twoWeek = _single_obj["twoWeek"].toBool(false);
        m_AllToDoList.append(_todolist_struct);
    }
}

void MainWindow::read_finished_data(const QString& file_path)
{
    QJsonArray _rootList = GetJsonDocumentFromFile(file_path).array();

    for(int i = 0; i < _rootList.size(); i++)
    {
        QJsonObject _single_obj = _rootList[i].toObject();
        QVector<ToDoSingleStruct> _finished_data;

        QString _character_name = _single_obj["name"].toString();
        double _character_score = _single_obj["score"].toDouble();
        QJsonArray _finish_status_array = _single_obj["finishStatus"].toArray();
        for(auto item : _finish_status_array)
        {
            QJsonObject _todo_single_obj = item.toObject();
            ToDoSingleStruct _todo_single_struct;
            _todo_single_struct.is_finished = _todo_single_obj["isFinish"].toBool();
            _todo_single_struct.name = _todo_single_obj["name"].toString();
            _finished_data.push_back(_todo_single_struct);
        }

        Character* _new_character = new Character(m_vect_character.size(), CompareData_IsFinished(_finished_data, m_AllToDoList), this, _character_name, _character_score);
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

void MainWindow::update_Config(const QByteArray& data)
{
    bool _is_overwrite = false;

    QFileInfo _fileInfo;
    _fileInfo.setFile(m_config_path);
    if(!_fileInfo.isFile())
    {
        _is_overwrite = true;
    }
    else
    {
        //副本设置文件存在，比较版本号，保留较新的文件

        QJsonDocument _old_json_doc = GetJsonDocumentFromFile(m_config_path);
        if(_old_json_doc.isEmpty())
        {
            _is_overwrite = true;
        }
        else
        {
            double _old_version_num = _old_json_doc.object()["version"].toDouble();
            QJsonDocument _new_json_doc = GetJsonDocumentFromByteArray(data);
            double _new_version_num = _new_json_doc.object()["version"].toDouble();
            if(_new_version_num > _old_version_num)
            {
                if(QMessageBox::question(this, "提示", "检测到新版本，是否跟新？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                {
                    _is_overwrite = true;
                }
            }
            else
            {
                QMessageBox::information(this, "提示", "已经是最新版本拉", QMessageBox::Yes);
            }
        }
    }


    if(_is_overwrite)
    {
        // 副本设置文件不存在或需要更新
        QFile _file(m_config_path);
        if (_file.open(QFile::WriteOnly | QFile::Truncate))
        {
            _file.write(data);
            _file.close();
        }

        read_AllToDoList(m_config_path);

        for(auto iter : m_vect_character)
        {
            iter->UpdateAllToDoListVector(m_AllToDoList);
        }
    }

}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QJsonArray _allCharacter_status_obj;
    for(auto item : m_vect_character)
    {
        _allCharacter_status_obj.append(item->GenSaveFile());
    }
    QJsonDocument _save_json(_allCharacter_status_obj);
    QFile _save_finish_data(m_finished_config_path);
    if(!_save_finish_data.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "保存文件打开出错";
    }
    else
    {
        _save_finish_data.write(_save_json.toJson());
        _save_finish_data.close();
    }


    QDir _dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    QString _existed_ini_path = _dir.absoluteFilePath(DEFAULTAPPCONFIGNAME);
    QSettings _app_setting(_existed_ini_path, QSettings::IniFormat);
    _app_setting.setValue("WindowGeometry/Height", this->height());
    _app_setting.setValue("WindowGeometry/Width", this->width());
    _app_setting.setValue("WindowGeometry/X", this->x());
    _app_setting.setValue("WindowGeometry/Y", this->y());

    _app_setting.setValue("DataConfig/RootPath", m_savefile_dir_path);
}

void MainWindow::OnUpdateToDoListFinished(const int& index_character, const ToDoSingleStruct& todo_list_struct)
{
    m_vect_character[index_character]->UpdateToDoListFinished(todo_list_struct);
}

void MainWindow::AddNewCharacter(Character* new_character)
{
    m_vect_character.push_back(new_character);
    connect(new_character, &Character::sig_character_checked, this, &MainWindow::OnUpdateToDoListUI);
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

}

void MainWindow::OnUpdateToDoListUI(const int& index_character, const QVector<ToDoSingleStruct>& todolist_str)
{
    clear_ToDoList();

    for(int i = 0; i < m_vect_character.size(); i++)
    {
        if(i != index_character)
        {
            m_vect_character[i]->setStyleSheet("");
        }
        else
        {
            m_vect_character[i]->setStyleSheet("background-color: rgb(215, 215, 215);border-radius:10px;");
        }
    }

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

void MainWindow::SetUpdateUrl(const QString& url)
{
    m_update_url = url;
}

void MainWindow::ClearAllFinished(bool is_two_week)
{
    for(int i = 0; i < m_vect_character.size(); i++)
    {
        m_vect_character[i]->ClearAllFinished(is_two_week);
    }
    clear_ToDoList();
}

void MainWindow::SetFinished()
{
    //设置更新文件地址
    m_downloadFile->SetUrl(m_update_url);

    QFileInfo _fileInfo;

    _fileInfo.setFile(m_config_path);
    if(!_fileInfo.isFile())
    {
        // 设置文件不存在
        // 从网络下载更新
        m_downloadFile->start();
    }
    else
    {
        read_AllToDoList(m_config_path);
    }

    _fileInfo.setFile(m_finished_config_path);
    if(_fileInfo.isFile())
    {
        read_finished_data(m_finished_config_path);
    }
}

void MainWindow::SetDefaultConfigDir(const QString& dir_path)
{
    m_savefile_dir_path = dir_path;
    QDir _dir(m_savefile_dir_path);
    if(!_dir.exists(m_savefile_dir_path))
    {
        _dir.mkdir(m_savefile_dir_path);
    }
    //默认副本设置文件保存地址
    m_config_path = _dir.absoluteFilePath(DEFAULTCONFIGNAME);
    //默认角色及副本完成情况保存地址
    m_finished_config_path = _dir.absoluteFilePath(DEFAULTFINISHEDDATANAME);
}


void MainWindow::on_open_config_dir_triggered()
{
    QDesktopServices::openUrl(QUrl("file:" + m_savefile_dir_path, QUrl::TolerantMode));
}


void MainWindow::on_clean_finished_status_triggered()
{
    QMessageBox _two_week_check_msgbox = QMessageBox(this);
    _two_week_check_msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Cancel);
    _two_week_check_msgbox.setWindowTitle("请求确认");
    _two_week_check_msgbox.setText("确定到周三了嘛，要清空了哦~\n选一下单周还是双周哦~");
    _two_week_check_msgbox.button(QMessageBox::Yes)->setText("单周");
    _two_week_check_msgbox.button(QMessageBox::YesToAll)->setText("双周");
    _two_week_check_msgbox.button(QMessageBox::Cancel)->setText("点错啦");

    switch(_two_week_check_msgbox.exec())
    {
        case QMessageBox::Yes:
            {
                //单周
                ClearAllFinished(false);
                break;
            }
        case QMessageBox::YesToAll:
            {
                //双周
                ClearAllFinished(true);
                break;
            }
        case QMessageBox::Cancel:
            {
                break;
            }
    }
}


void MainWindow::on_add_character_triggered()
{
    Character* _new_character = new Character(m_vect_character.size(), m_AllToDoList, this);
    AddNewCharacter(_new_character);
}


void MainWindow::on_update_config_triggered()
{
    m_downloadFile->start();
}


void MainWindow::on_open_app_config_dir_triggered()
{
    QDesktopServices::openUrl(QUrl("file:" + QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation), QUrl::TolerantMode));
}


void MainWindow::on_overview_triggered()
{
    QVector<CharacterToDoListStruct> _tmp_data;

    for(const auto& item : m_vect_character)
    {

        CharacterToDoListStruct _single_character_data;
        _single_character_data.nick_name = item->GetCharaterNickName();
        _single_character_data.character_score = item->GetCharacterScore();
        _single_character_data.todo_struct_vect = item->GetUnfinishedData();
        if(_single_character_data.todo_struct_vect.isEmpty())
            continue;
        _tmp_data.push_back(_single_character_data);
    }
    Overview::GetInstance()->SetUnfinishedData(_tmp_data);
    Overview::GetInstance()->show();
}


void MainWindow::on_actionAbout_triggered()
{
    About::GetInstance()->show();
}

