#ifndef ANDEME_COMMON_SQLITE_UTIL_H
#define ANDEME_COMMON_SQLITE_UTIL_H
#include "schema/message.pb.h"
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <memory>

namespace andeme {

    class SQLite_util
    {
    public:
        //База
        SQLite_util();
        //запрещаем копирование
        SQLite_util(SQLite_util const &) = delete;
        SQLite_util& operator=(SQLite_util const &) = delete;

        ~SQLite_util();
        //команды SQL
        bool createTable();
        bool insert(const std::string&);
        bool show(const std::string&);
        static int callback(void *NotUsed, int argc, char **argv, char **azColName);

    private:
        sqlite3* m_db;
    };

    class MessageStorage
    {
    public:
        class SQLite_util;

        MessageStorage();
        bool add(/*const andeme::schema::Message& msg*/); // возвращает true если это новое сообщение, false если уже есть
        void getAll(/*const std::function<bool(const andeme::schema::Message&)>& callback*/) const; // передаёт в callback по одному все сообщения
    private:
        std::unique_ptr<andeme::SQLite_util> base;
    };
}
#endif // ANDEME_COMMON_SQLITE_UTIL_H
