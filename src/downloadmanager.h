#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QDebug>
#include <QDir>

#include <stdio.h>

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

//void start_parse(QString t_url);

class DownloadManager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;

public:
    DownloadManager();
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    QString savePathName();
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void downloadFinished(QNetworkReply *reply);

signals:
    void done();
    void downloadFailed(QByteArray, QString);
    void downloadEnded(QByteArray);

};


#endif // DOWNLOADMANAGER_H
