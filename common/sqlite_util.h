/*#ifndef ANDEME_COMMON_SQLITE_UTIL_H
#define ANDEME_COMMON_SQLITE_UTIL_H
#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <schema/message.pb.h>

namespace andeme {

    using Row = std::vector<std::string>;
    using Callback = std::function<bool(const std::vector<std::string>&)>;

    class SQLite3Storage {
    protected:

        SQLite3Storage(const std::string&);
        bool execute(const std::string& query, const Callback& callback);

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
         MessageStorage(const std::string& dbname);
         bool add(const andeme::schema::Message& msg); // возвращает true если это новое сообщение, false если уже есть
         std::vector<andeme::schema::Message> getAllMsg(const std::function<bool(const andeme::schema::Message&)>& callback); // передаёт в callback по одному все сообщения
    private:
    };

        class SqliteGuard {
    public:
        SqliteGuard() {
            sqlite3_initialize();
        }

        ~SqliteGuard() {
            std::cout << "shutdown" << std::endl;
            sqlite3_shutdown();
        }
    };

    int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName);

}

#endif // ANDEME_COMMON_SQLITE_UTIL_H
*/
