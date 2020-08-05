#pragma once
#include "sqliteutil.h"


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
        execute(sql.data(),std::vector<andeme::schema::Message>()/*nullptr*/);
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
        std::string Author = "Author";
        std::string sign = "sign";
        std::string sql =  "INSERT INTO MESSAGES ('Timestamp', 'Author', 'Message','Signature') \
                VALUES (datetime('now','localtime'),'"+ Author +"','"+ msg.text() +"','"+ sign +"');";
        return (execute(sql.data(),std::vector<andeme::schema::Message>()/*nullptr*/));
    }

    std::vector<andeme::schema::Message> MessageStorage::getAllMessages()
    {
        Callback callback;

        execute("SELECT Message FROM 'MESSAGES' ", callback);
            return callback;
    }

    int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        andeme::Callback& callback = *static_cast<Callback*>(NotUsed);

            andeme::schema::Message result;

            for (size_t i = 0; i < argc; ++i) {
                result.set_text(argv[i]);
                callback.emplace_back(result);
            }
            if (!callback.empty())
                return SQLITE_OK;
            else
                return SQLITE_FAIL;
    }
}
