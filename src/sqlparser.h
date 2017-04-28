#ifndef SQLPARSER_H
#define SQLPARSER_H
#include <QObject>

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#define TESTCLASS_H

#include "downloadmanager.h"

#include <QGuiApplication>
#include <QQuickView>
#include <QtQml>
#include <QUrl>

#include <string.h>
#include <QString>
#include <fstream>
#include <iostream>
#include <vector>
#include <sqlite3.h>

struct Dic_data{
    std::string word;
    std::string information;

public:
    Dic_data(){
        word = "test";
        information = "info";
    }
    Dic_data(std::string w, std::string i = ""){
        this->word = w;
        this->information = i;
    }

    void set_info(std::string i){
        this->information = i;
    }

    std::string get_info()
    {
        return this->information;
    }

    std::string get_word()
    {
        return this->word;
    }
};

class SQLParser: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_INVOKABLE std::ifstream dicfile;
    Q_INVOKABLE std::vector<Dic_data> dic_data;

    bool m_busy;
    DownloadManager *m_downloadManager;

public:
    Q_INVOKABLE SQLParser();

    Q_INVOKABLE void start_parse(QString file);
    Q_INVOKABLE QString downloadDict(QString dictionaryUrl);
    Q_INVOKABLE bool showNotification(QString category, const QString summary, const QString text, QString previewBody, QString previewSummary, QString icon);
    //Q_INVOKABLE void deleteDictionary(QString dict);
    Q_INVOKABLE QString get_item_info(int i);
    /*Q_INVOKABLE*/ bool busy() const;
    /*Q_INVOKABLE*/ void setBusy(bool busy);

public slots:
    void downloadDone();
    void downloadError(QByteArray url, QString errorMsg);
    void downloadEnded(QByteArray url);

signals:
    void busyChanged();
};

#endif // SQLPARSER_H
