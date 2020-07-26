#ifndef ANDEME_COMMON_SQLITE_UTIL_H
#define ANDEME_COMMON_SQLITE_UTIL_H
#include <sqlite3.h>
#include <string>
#include <memory>
#include <iostream>
#include "message.pb.h"

namespace andeme {

    class SQLite3Storage {
    public:
        friend class MessageStorage;
        static SQLite3Storage& Instance()
        {
            static SQLite3Storage mpoSssInstance;
            return mpoSssInstance;
        }
        bool execute(const std::string& query/*, const Callback& callback*/);
        void SetBaseName(const std::string& filename);

    protected:
        static SQLite3Storage* mpoSssInstance;
        SQLite3Storage();
        ~SQLite3Storage();
        //copying is prohibited
        SQLite3Storage(SQLite3Storage const &) = delete;
        SQLite3Storage& operator=(SQLite3Storage const &) = delete;

    private:
       sqlite3* m_db;
       std::string m_name;
    };


    class MessageStorage : public SQLite3Storage
    {
    public:
         bool add(/*const andeme::schema::Message& msg*/); // возвращает true если это новое сообщение, false если уже есть
         void getAll(/*const std::function<bool(const andeme::schema::Message&)>& callback*/); // передаёт в callback по одному все сообщения
    private:
    };



}

#endif // ANDEME_COMMON_SQLITE_UTIL_H
