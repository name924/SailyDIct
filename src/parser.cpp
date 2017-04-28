#include "parser.h"
#include <QDebug>

Parser::Parser()
{
    ;
}

Parser::~Parser()
{
    ;
}

void Parser::parse(QString file)
{
    /*
    qDebug()<<"DOWNLOAD MANAGER PARSING!";
    std::string url = t_url.toStdString();
    std::string filename = url.substr(url.rfind('/') + 1);
    std::string dirname = filename.substr(0, filename.rfind('.'));
    std::string dictname = dirname.substr(9, dirname.rfind('-') - 9) + ".dict";
    std::string cmd = "cd .local/share/Dictionaries/ && unzip " + filename + " && rm " + filename +
                      " && cp " + dirname + '/' + dictname + ".dz " + dictname + ".gz && gunzip -f " + dictname + ".gz" +
                      " && rm -rf " + dirname;
    qDebug() << QString::fromStdString(cmd);
    system(cmd.c_str());

    QString file = QString::fromStdString(dictname);
    */
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    std::ifstream dicfile;
    std::vector<Dic_data> dic_data;

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
    rc = sqlite3_open("/home/nemo/.local/share/TestSQL/TestSQL/QML/OfflineStorage/Databases/a708e8b63b170c5319af5a6423c8c44f.sqlite", &db);
    if( rc ){
        //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        qDebug() << "Can't open database: " + QString::fromStdString(sqlite3_errmsg(db));
        return;
    }else{
        //fprintf(stderr, "Opened database successfully\n");
        qDebug() << "Opened database successfully";
    }

    //Create SQL table
    sql = "CREATE TABLE IF NOT EXISTS Entries ("  \
             "id INTEGER PRIMARY KEY AUTOINCREMENT," \
             "word           TEXT    NOT NULL," \
             "info           TEXT," \
             "dict           TEXT    NOT NULL, "\
             "FOREIGN KEY(dict) REFERENCES Dicts(dict) ON DELETE CASCADE);"\
             "CREATE VIRTUAL TABLE IF NOT EXISTS EntriesSearch USING fts4(id, word, info, dict);" \
             "CREATE TABLE IF NOT EXISTS Dicts (dict TEXT PRIMARY KEY, isActive INTEGER NOT NULL);";

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

    emit finished();
}

int Parser::callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void Parser::RunDeleteParamSQL(sqlite3 *db, char *szSQL, std::string to_del)
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

void Parser::RunInsertWordsSQL(sqlite3 *db, std::vector<char *> fn, std::vector<char *> ln, std::string dict_type)
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

void Parser::RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn)
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

void Parser::RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn, int a)
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
