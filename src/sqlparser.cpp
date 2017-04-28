#include "sqlparser.h"
#include <cstring>
#include <QString>
#include <QStringListModel>
#include <QMultiHash>
#include <QModelIndex>
#include <QSettings>
#include <QFile>
#include <QtDBus/QtDBus>
#include <QThread>
#include "parser.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void RunDeleteParamSQL(sqlite3 *db, char *szSQL, std::string to_del)
{
  if (!db)
    return;

  char *del = new char[to_del.length() + 1];
  std::strcpy(del, to_del.c_str());

  char *zErrMsg = 0;
  sqlite3_stmt *stmt;
  const char *pzTest;
  //char *szSQL;

  //szSQL = "DELETE FROM Entries WHERE dict = ?";

  int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

  if( rc == SQLITE_OK ) {
    // bind the value
    sqlite3_bind_text(stmt, 1, del, strlen(del), 0);

    // commit
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
}

void RunInsertWordsSQL(sqlite3 *db, std::vector<char *> fn, std::vector<char *> ln, std::string dict_type)
{
    if (!db)
        return;

    char *d_type = new char[dict_type.length() + 1];
    std::strcpy(d_type, dict_type.c_str());

    char *zErrMsg = 0;
    sqlite3_stmt *stmt;
    const char *pzTest;
    std::string sql = "insert into Entries (word, info, dict) values (?,?,?)";

    // Insert data item into Table
    for (int i = 1; i < fn.size(); ++i)
        sql += ", (?,?,?)";

    char *szSQL = new char[sql.length() + 1];
    std::strcpy(szSQL, sql.c_str());
    //qDebug()<< QString::fromStdString(szSQL);
    int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

    if( rc == SQLITE_OK ) {
        // bind the value
        for (int i = 0; i < fn.size(); ++i)
        {
            sqlite3_bind_text(stmt, i*3 + 1, fn[i], strlen(fn[i]), 0);
            sqlite3_bind_text(stmt, i*3 + 2, ln[i], strlen(ln[i]), 0);
            sqlite3_bind_text(stmt, i*3 + 3, d_type, strlen(d_type), 0);
        }

        // commit
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn)
{
  if (!db)
    return;

  char *zErrMsg = 0;
  sqlite3_stmt *stmt;
  const char *pzTest;
  //char *szSQL;

  // Insert data item into myTable
  //szSQL = "insert into Dicts (dict) values (?)";

  int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

  if( rc == SQLITE_OK ) {
    // bind the value
    sqlite3_bind_text(stmt, 1, fn, strlen(fn), 0);

    // commit
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
}

void RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn, int a)
{
  if (!db)
    return;

  char *zErrMsg = 0;
  sqlite3_stmt *stmt;
  const char *pzTest;
  //char *szSQL;

  // Insert data item into myTable
  //szSQL = "insert into Dicts (dict) values (?)";

  int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

  if( rc == SQLITE_OK ) {
    // bind the value
    sqlite3_bind_text(stmt, 1, fn, strlen(fn), 0);
    sqlite3_bind_int(stmt, 2, a);

    // commit
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
}

SQLParser::SQLParser()
{
    m_downloadManager = new DownloadManager();

    connect(m_downloadManager,
            SIGNAL(done()),
            this,
            SLOT(downloadDone()));

    connect(m_downloadManager,
            SIGNAL(downloadFailed(QByteArray, QString)),
            this,
            SLOT(downloadError(QByteArray, QString)));

    connect(m_downloadManager,
            SIGNAL(downloadEnded(QByteArray)),
            this,
            SLOT(downloadEnded(QByteArray)));
    //system(">a.txt");
    //start_parse();
}


QString SQLParser::downloadDict(QString dictionaryUrl)
{
    std::string url = dictionaryUrl.toStdString();
    std::string filename = url.substr(url.rfind('/') + 1);
    std::string dirname = filename.substr(0, filename.rfind('.'));
    std::string dictname = dirname.substr(9, dirname.rfind('-') - 9) + ".dict";
    /*
    std::string cmd = "mkdir -p Dictionaries && cd Dictionaries && curl -O " + url +
                      "&& unzip " + filename + " && rm " + filename +
                      " && cp " + dirname + '/' + dictname + ".dz " + dictname + ".gz && gunzip -f " + dictname + ".gz" +
                      " && rm -rf " + dirname;
    //qDebug() << QString::fromStdString(cmd);
    system(cmd.c_str());
    */
    m_downloadManager->doDownload(QUrl(dictionaryUrl));
    return QString::fromStdString(dictname);
}

void SQLParser::start_parse(QString file)
{

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    int i = 0;
    std::string dict_name = file.toStdString().substr(5, file.toStdString().rfind('.') - 5);
    std::string inf = "";
    std::string qqq = "/home/nemo/.local/share/Dictionaries/" + file.toStdString();
    char *qqqq = new char [qqq.length()];
    std::strcpy(qqqq, qqq.c_str());
    //dicfile.open("/home/nemo/Dictionaries/" + file.toStdString());
    dicfile.open(qqqq);
    std::string line;

    std::getline(dicfile, line);
    int s = line.find(">") + 1;
    int e = line.find("<", s);
    //qDebug() << QString::fromStdString(line.substr(s, e - s));
    dic_data.clear();
    dic_data.push_back(Dic_data(line.substr(s, e - s)));

    while (std::getline(dicfile, line))
    {
        //qDebug() << QString::fromStdString(line);
        if (line.find("<k>") != -1){
            s = line.find(">") + 1;
            e = line.find("<", s);
            inf += line.substr(0, line.find("<"));
            //qDebug() << "inf: " << QString::fromStdString(inf);
            dic_data.at(i).set_info(inf.substr(inf.find('\n') + 1));
            inf = "";
            //qDebug() << "inf: " << QString::fromStdString(inf);
            i += 1;
            dic_data.push_back(Dic_data(line.substr(s, e - s)));
            //qDebug() << QString::fromStdString(line.substr(s, e - s));
        }
        else{
            inf += line + '\n';
        }
    }
    dicfile.close();
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;

    // Open database
    rc = sqlite3_open("/home/nemo/.local/share/SailyDict/SailyDict/QML/OfflineStorage/Databases/e83122e0694af4a7286e4f36b7047266.sqlite", &db);
    if( rc ){
        //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        qDebug() << "Can't open database: " + QString::fromStdString(sqlite3_errmsg(db));
        return;
    }else{
        //fprintf(stderr, "Opened database successfully\n");
        qDebug() << "Opened database successfully";
    }

    char *sql_t = "PRAGMA auto_vacuum = FULL;";
    rc = sqlite3_exec(db, sql_t, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK ) {
            //fprintf(stderr, "Failed to select data\n");
            //fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return;
    }

    //Create SQL table

    sql = "CREATE TABLE IF NOT EXISTS Dicts (dict TEXT PRIMARY KEY, isActive INTEGER NOT NULL);"\
          "CREATE TABLE IF NOT EXISTS Entries ("  \
          "id INTEGER PRIMARY KEY AUTOINCREMENT," \
          "word           TEXT    NOT NULL," \
          "info           TEXT," \
          "dict           TEXT    NOT NULL, "\
          "CONSTRAINT fk_dicts FOREIGN KEY(dict) REFERENCES Dicts(dict) ON DELETE CASCADE);"\
          "CREATE VIRTUAL TABLE IF NOT EXISTS EntriesSearch USING fts4(id, word, info, dict);";

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        qDebug() << "SQL error: " + QString::fromStdString(zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        //fprintf(stdout, "Table created successfully\n");
        qDebug() << "Tables created successfully";
    }

    //Insert dictionary name to Dicts table
    char *dname = new char[dict_name.length()];
    std::strcpy(dname, dict_name.c_str());
    RunInsertParamSQL(db, "insert into Dicts values (?, ?)", dname, 1);

    //Delete previous dictionary data
    qDebug() << QString::fromStdString(dict_name);
    RunDeleteParamSQL(db, "DELETE FROM Entries WHERE dict = (?)", dict_name);
    RunDeleteParamSQL(db, "DELETE FROM EntriesSearch WHERE dict = (?)", dict_name);

    // Insert data from dictionary
    std::vector<char *> vw;
    std::vector<char *> vin;

    for (int q = 0; q < dic_data.size(); ++q)
    {
        char *w = new char[dic_data.at(q).get_word().length() + 1];
        char *in = new char[dic_data.at(q).get_info().length() + 1];

        std::strcpy(w, dic_data.at(q).get_word().c_str());
        std::strcpy(in, dic_data.at(q).get_info().c_str());

        vw.push_back(w);
        vin.push_back(in);

        if (q != 0 && q % 250 == 0)
        {
            RunInsertWordsSQL(db, vw, vin, dict_name);
            vw.clear();
            vin.clear();
            qDebug() << q;
            //sqlite3_db_cacheflush(db);
            //return;
        }

    }
    if (!vin.empty())
        RunInsertWordsSQL(db, vw, vin, dict_name);

    RunInsertParamSQL(db, "INSERT INTO EntriesSearch SELECT id, word, info, dict FROM Entries WHERE dict = (?)", dname);

    sqlite3_close(db);
    qDebug() << "Data added successfully";

    //setBusy(false);

    //qDebug()<<"WRONG PARSER!";
}

bool SQLParser::busy() const
{
    return m_busy;
}

void SQLParser::setBusy(bool busy)
{
    m_busy = busy;
    emit busyChanged();
}

void SQLParser::downloadDone()
{
    //IN;
    qDebug() << "all downloads finished";

//    showNotification("x-fi.rmz.sidudict.download",
//                     "All Sidudict downloads finished.",
//                     "All queued downloads for Sidudict finished.",
//                     "All downloads finished.",
//                     "All Sidudict downloads finished.",
//                     "icon-s-update");
}

void SQLParser::downloadError(QByteArray url, QString errorMsg)
{
    //IN;
    qDebug() << "Download Error:" << url;
    showNotification("x-fi.rmz.sidudict.download",
                     "Download failed!",
                     "Failed to download: " + url + " Error: " + errorMsg,
                     "Download failed!",
                     "File could not be downloaded!",
                     "icon-system-warning");
}

void SQLParser::downloadEnded(QByteArray url_g)
{
    //IN;
    qDebug() << "Download Ended:" << url_g;

    showNotification("x-fi.rmz.sidudict.download",
                     "Download finished!",
                     "Please wait for dictionary being processed!",
                     "Download finished!",
                     "Please wait for dictionary being processed!",
                     "icon-s-update");


    //updateDictCataqDebugue();
    QString t_url = QString::fromLocal8Bit(url_g);
    std::string url = t_url.toStdString();
    std::string filename = url.substr(url.rfind('/') + 1);
    std::string dirname = filename.substr(0, filename.rfind('.'));
    std::string dictname = dirname.substr(9, dirname.rfind('-') - 9) + ".dict";
    std::string cmd = "cd .local/share/Dictionaries/ && unzip " + filename + " && rm " + filename +
                      " && cp " + dirname + '/' + dictname + ".dz " + dictname + ".gz && gunzip -f " + dictname + ".gz" +
                      " && rm -rf " + dirname;
    qDebug() << QString::fromStdString(cmd);
    system(cmd.c_str());

    start_parse(QString::fromStdString(dictname));

    /*
    QThread* thread = new QThread;
    Parser* parser = new Parser();
    parser->moveToThread(thread);
    //connect(parser, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), parser, SLOT(parse(QString::fromStdString(dictname))));
    connect(parser, SIGNAL(finished()), thread, SLOT(quit()));
    connect(parser, SIGNAL(finished()), parser, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    */

    showNotification("x-fi.rmz.sidudict.download",
                     "Processing finished!",
                     "Now you can use this dictionary!",
                     "Processing finished!",
                     "Now you can use this dictionary!",
                     "icon-s-update");

}

bool SQLParser::showNotification(const QString category,
                                   const QString summary,
                                   const QString text,
                                   const QString previewSummary,
                                   const QString previewBody,
                                   const QString icon)
{
    //IN;
    QVariantMap hints;
    hints.insert("category", category);
    hints.insert("x-nemo-preview-body", previewBody);
    hints.insert("x-nemo-preview-summary", previewSummary);
    QList<QVariant> argumentList;
    argumentList << "SailyDict";    //app_name
    argumentList << (uint)0;       // replace_id
    argumentList << icon;          // app_icon
    argumentList << summary;       // summary
    argumentList << text;          // body
    argumentList << QStringList(); // actions
    argumentList << hints;         // hints
    argumentList << (int)5000;     // timeout in ms

  static QDBusInterface notifyApp("org.freedesktop.Notifications",
                                  "/org/freedesktop/Notifications",
                                  "org.freedesktop.Notifications");
  QDBusMessage reply = notifyApp.callWithArgumentList(QDBus::AutoDetect,
                                                      "Notify", argumentList);

  if(reply.type() == QDBusMessage::ErrorMessage) {
    qDebug() << "D-Bus Error:" << reply.errorMessage();
    return false;
  }
  return true;
}

QString SQLParser::get_item_info(int i){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    return QString::fromStdString(dic_data.at(i).get_word()) + "    " + QString::fromStdString(dic_data.at(i).get_info());
}
