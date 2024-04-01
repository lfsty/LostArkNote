#ifndef BASEFUNCTION_H
#define BASEFUNCTION_H

#include "INFO.h"
#include <QVector>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

// 给所有数据赋上是否完成标签
QVector<ToDoSingleStruct> CompareData_IsFinished(const QVector<ToDoSingleStruct>& finished_status, const QVector<ToDoSingleStruct>& all_data);
// 从文件中读取json文件
const QJsonDocument GetJsonDocumentFromFile(const QString& file_path);
// 从字节流中读取json文件
const QJsonDocument GetJsonDocumentFromByteArray(const QByteArray& data);
#endif // BASEFUNCTION_H
