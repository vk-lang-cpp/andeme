#include "sqlite_util.h"


namespace andeme {
    SQLite_util::SQLite_util() {

        sqlite3_initialize();
        sqlite3_open_v2("Andeme.db", &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        createTable();
    }

    SQLite_util::~SQLite_util()
    {
        sqlite3_shutdown();
        sqlite3_close(m_db);
    }

    bool SQLite_util::createTable()
    {
        std::string sql = "CREATE table IF NOT EXISTS MESSAGES ("  \
            "ID INT     NOT NULL," \
            "Date		TEXT	NOT NULL," \
            "MESSAGE	TEXT    NOT NULL,UNIQUE(Date,MESSAGE));";
        return sqlite3_exec(m_db, sql.data(), nullptr, 0, nullptr);
    }

    bool SQLite_util::insert(const std::string& sql)
    {
        if (sqlite3_exec(m_db, sql.c_str(), nullptr, 0, nullptr) != SQLITE_OK)
            return false;
        return true;
    }

    bool SQLite_util::show(const std::string& sql)
    {

        // Run the SQL
        if( sqlite3_exec(m_db, sql.data(), callback, 0, nullptr) != SQLITE_OK)
            return false;
        return true;
    }

    int SQLite_util::callback(void *NotUsed, int argc, char **argv, char **azColName) //подправить под свои нужды
    {

        for (int i = 0; i < argc; i++) {

            // Show column name, value, and newline
            std::cout  << argv[i]<<" ";
        }
        std::cout << std::endl;

        // Return successful
        return 0;
    }

    MessageStorage::MessageStorage():base(new andeme::SQLite_util)
    {
    }

    bool MessageStorage::add(/*const andeme::schema::Message & msg*/)
    {
        //std::string message(msg.text());
        base->insert("INSERT INTO MESSAGES ('ID', 'Date', 'MESSAGE') VALUES ('14',datetime('now','localtime'),'Hello');");
        return false;
    }

    void MessageStorage::getAll(/*const std::function<bool(const andeme::schema::Message&)>& callback*/) const
    {
        base->show("SELECT * FROM 'MESSAGES';");
    }
}
