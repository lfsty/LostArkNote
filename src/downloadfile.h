#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class DownloadFile : public QThread
{
    Q_OBJECT

public:
    explicit DownloadFile(QObject* parent = nullptr);

private:
    QString m_Url;
public:
    void SetUrl(const QString& url);

protected:
    void run() override;

signals:
    void downloadFinished(const QByteArray& data);
};

#endif // DOWNLOADFILE_H
