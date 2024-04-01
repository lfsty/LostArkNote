#include "downloadfile.h"

#include <QDebug>
DownloadFile::DownloadFile(QObject* parent)
    : QThread{parent}
{
}

void DownloadFile::SetUrl(const QString& url)
{
    m_Url = url;
}

void DownloadFile::run()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(m_Url);
    QEventLoop loop;
    QNetworkReply* reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            emit downloadFinished(reply->readAll());
        }
        else
        {
            qDebug() << reply->error();
        }
        reply->deleteLater();
        loop.quit();
    });
    // move reply object to this thread
    reply->moveToThread(this);
    loop.exec();
}

