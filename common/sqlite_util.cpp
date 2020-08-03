#pragma once
#include "sqlite_util.h"


namespace andeme {

    SQLite3Storage::SQLite3Storage(const std::string &filename)
    {
        static SqliteGuard guard;

        std::string sql = "CREATE table IF NOT EXISTS MESSAGES ("  \
            "Timestamp	INTEGER		NOT NULL," \
            "Author		TEXT		NOT NULL," \
            "Message	TEXT		NOT NULL," \
            "Signature	TEXT		NOT NULL ,UNIQUE(Timestamp,Message));";

        sqlite3_open_v2(filename.data(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        execute(sql.data(),nullptr);
    }

    SQLite3Storage::~SQLite3Storage()
    {
        sqlite3_close_v2(m_db);
    }

    bool SQLite3Storage::execute(const std::string& query,const Callback& callback)
    {
        return (SQLITE_OK == sqlite3_exec(m_db, query.c_str(), sqlite_callback, (void *)(&callback), nullptr));
    }

    MessageStorage::MessageStorage(const std::string & dbname):SQLite3Storage(dbname){}

    bool MessageStorage::add(const andeme::schema::Message & msg)
    {
        std::string message/*(msg.text())*/ = "Test";
        std::string Author = "Author";
        std::string sign = "sign";
        std::string sql =  "INSERT INTO MESSAGES ('Timestamp', 'Author', 'Message','Signature') \
                VALUES (datetime('now','localtime'),'"+ Author +"','"+ message +"','"+ sign +"');";
        return (execute(sql.data(),nullptr));
    }

    std::vector<andeme::schema::Message> MessageStorage::getAllMsg
                                        (const std::function<bool(const andeme::schema::Message&)>& callback)
    {
        execute("SELECT * FROM 'MESSAGES';", nullptr);
        return std::vector<andeme::schema::Message>(); //заглушка
    }

    int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        const andeme::Callback& callback = *static_cast<Callback*>(NotUsed);

        std::vector<std::string> result;
        result.reserve(argc);

        for (size_t i = 0; i < argc; ++i) {
            result.emplace_back(argv[i]);
        }

        if (callback(result)) {
            return 1;
        }
        else {
            return 0;
        }
    }

}
