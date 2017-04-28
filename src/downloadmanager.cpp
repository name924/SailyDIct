#include "downloadmanager.h"
//#include "JlCompress.h"

DownloadManager::DownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadManager::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    const QString pathName = savePathName();
    QString savePathName;
    savePathName.append(pathName);
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    savePathName.append(basename);

    qDebug() << "save Path Name:" << savePathName;

    return savePathName;
}

QString DownloadManager::savePathName()
{
    QByteArray xdg_data_home = qgetenv("XDG_DATA_HOME");
    QString saveLocation;
    if (xdg_data_home.isEmpty()) {
        //saveLocation = QDir::homePath() + QString("/.local/share/harbour-sidudict/");
        saveLocation = QDir::homePath() + QString("/.local/share/Dictionaries/");
    } else {
        saveLocation = QString("%1/Dictionaries/").arg(xdg_data_home.constData());
    }

    QDir savePathNameDir(saveLocation);
    if (!savePathNameDir.exists()){
        qDebug() << "path does not exist:" << saveLocation;
        if (savePathNameDir.mkpath(saveLocation)){
            qDebug() << "created path:" << saveLocation;
        } else {
            qDebug() << "path creation failed:" << saveLocation;
        }
    }

    return saveLocation;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    //IN;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open" << qPrintable(filename) << "for writing:" << qPrintable(file.errorString());
        return false;
    }

    if (file.write(data->readAll()) > 0) {
        file.close();
        //QStringList extractedList = JlCompress::extractDir(filename, savePathName());
        //file.remove();
        //qDebug() << "Extracted:" << extractedList;
        return true;
        //if (extractedList.size() > 0) {
        //    return true;
        //} else {
        //    return false;
        //}
    } else {
        file.close();
        return false;
    }
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    //IN;
    QUrl url = reply->url();

    if (reply->error()) {
        qDebug() << "Download of" << url.toEncoded().constData() << "failed:" << qPrintable(reply->errorString());
        emit downloadFailed(url.toEncoded(), reply->errorString());
    } else {
        int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatusCode == 200) {
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply)){
                qDebug() << "Download of" << url.toEncoded().constData() << "succeded saved to:"  << qPrintable(filename);

                //start_parse(url.toString());

                emit downloadEnded(url.toEncoded());
            } else {
                emit downloadFailed(url.toEncoded(), "Could not save file!");
            }
        } else if (httpStatusCode == 301) {
            QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            qDebug() << url << ": 301 Moved Permanently to:";
            qDebug() << redirectUrl;
            doDownload(redirectUrl);
        } else {
            emit downloadFailed(url.toEncoded(), QString("Could not handle HTTP status Code: %f").arg(httpStatusCode));
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty()){
        // all downloads finished
        emit done();
    }

}
