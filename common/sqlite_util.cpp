#pragma once
#include "sqlite_util.h"


namespace andeme {


    SQLite3Storage::SQLite3Storage()
    {
        std::cout << "constructor\n";
        sqlite3_initialize();

    }

    SQLite3Storage::~SQLite3Storage()
    {
        std::cout << "destructor\n";
        sqlite3_shutdown();
        sqlite3_close(m_db);
    }


    bool SQLite3Storage::execute(const std::string& query/*,const Callback& callback*/)
    {
        if (sqlite3_exec(m_db, query.c_str(), nullptr, 0, nullptr) != SQLITE_OK)
            return false;
        return true;
    }

    void SQLite3Storage::SetBaseName(const std::string& filename)
    {
        m_name=filename;
        sqlite3_open_v2(filename.data(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        execute("CREATE table IF NOT EXISTS MESSAGES ("  \
            "ID INT     NOT NULL," \
            "Date		TEXT	NOT NULL," \
            "MESSAGE	TEXT    NOT NULL,UNIQUE(Date,MESSAGE));"/*,nullptr*/);
    }


    bool MessageStorage::add(/*const andeme::schema::Message & msg*/)
    {
        //std::string message(msg.text());
        return (this->execute("INSERT INTO MESSAGES ('ID', 'Date', 'MESSAGE') \
                VALUES ('14',datetime('now','localtime'),'Hello');"/*,nullptr*/));

    }

    void MessageStorage::getAll(/*const std::function<bool(const andeme::schema::Message&)>& callback*/)
    {
        execute("SELECT * FROM 'MESSAGES';"/*,callback*/);
    }


    int callback(void *NotUsed, int argc, char **argv, char **azColName) //подправить под свои нужды
    {

        for (int i = 0; i < argc; i++) {

            // Show column name, value, and newline
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        // Return successful
        return 0;
    }

}
