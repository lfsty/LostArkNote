#include "basefunction.h"

QVector<ToDoSingleStruct> CompareData_IsFinished(const QVector<ToDoSingleStruct>& finished_status, const QVector<ToDoSingleStruct>& all_data)
{
    if(all_data.empty())
    {
        return finished_status;
    }
    else
    {
        QVector<ToDoSingleStruct> _ret_data = all_data;

        for(auto iter_ret = _ret_data.begin(); iter_ret != _ret_data.end(); iter_ret++)
        {
            for(auto item_finished : finished_status)
            {
                if(iter_ret->name == item_finished.name)
                {
                    iter_ret->is_finished = item_finished.is_finished;
                    continue;
                }
            }
        }

        return _ret_data;
    }
}

const QJsonDocument GetJsonDocumentFromFile(const QString& file_path)
{
    QFile _loadFile(file_path);

    if(!_loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败" << file_path;
        return QJsonDocument();
    }
    QByteArray _allData = _loadFile.readAll();
    _loadFile.close();

    return GetJsonDocumentFromByteArray(_allData);
}

const QJsonDocument GetJsonDocumentFromByteArray(const QByteArray& data)
{
    QJsonParseError _jsonError;
    QJsonDocument _jsonDoc(QJsonDocument::fromJson(data, &_jsonError));
    if(_jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << _jsonError.errorString();
        return QJsonDocument();
    }

    return _jsonDoc;
}
