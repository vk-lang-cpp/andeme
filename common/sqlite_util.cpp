#pragma once
#include "sqlite_util.h"


namespace andeme {


    SQLite3Storage::SQLite3Storage(const std::string &filename)
    {
        static SqliteGuard guard;
        sqlite3_open_v2(filename.data(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        execute("CREATE table IF NOT EXISTS MESSAGES ("  \
            "ID INT     NOT NULL," \
            "Date		TEXT	NOT NULL," \
            "MESSAGE	TEXT    NOT NULL,UNIQUE(Date,MESSAGE));",nullptr);
    }

    bool SQLite3Storage::execute(const std::string& query,const Callback& callback)
    {
        return (SQLITE_OK == sqlite3_exec(m_db, query.c_str(), sqlite_callback, (void *)(&callback), nullptr));
    }


    MessageStorage::MessageStorage(const std::string & dbname):SQLite3Storage(dbname)
    {
    }

    bool MessageStorage::add(const andeme::schema::Message & msg)
    {
        //std::string message(msg.text());
        return (this->execute("INSERT INTO MESSAGES ('ID', 'Date', 'MESSAGE') \
                VALUES ('14',datetime('now','localtime'),'Hello');",nullptr));

    }

    std::vector<andeme::schema::Message> MessageStorage:: getAllMsg(const std::function<bool(const andeme::schema::Message&)>& callback)
    {
        execute("SELECT * FROM 'MESSAGES';", nullptr/*callback*/);
        return std::vector<andeme::schema::Message>();
    }


    int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        const andeme::Callback& callback = *static_cast<Callback*>(NotUsed);

        // argc - количество столбцов
        // argv - значения
        // azColName - имена столбцов

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
