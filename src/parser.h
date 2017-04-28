#ifndef PARSER_H
#define PARSER_H

#include <QTextCodec>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <QThread>

class Parser : public QObject
{
    Q_OBJECT

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

public:
    Parser();
    ~Parser();

public slots:
    void parse(QString file);

signals:
    void finished();
    void error(QString err);

private:
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    void RunDeleteParamSQL(sqlite3 *db, char *szSQL, std::string to_del);
    void RunInsertWordsSQL(sqlite3 *db, std::vector<char *> fn, std::vector<char *> ln, std::string dict_type);
    void RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn);
    void RunInsertParamSQL(sqlite3 *db, char *szSQL, char *fn, int a);

};

#endif // PARSER_H
