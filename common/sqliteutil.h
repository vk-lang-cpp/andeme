#ifndef ANDEME_COMMON_SQLITE_UTIL_H
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
        ~SQLite3Storage();
        bool execute(const std::string& query, const Callback&);

        //copying is prohibited
        SQLite3Storage(SQLite3Storage const &) = delete;
        SQLite3Storage& operator=(SQLite3Storage const &) = delete;

        sqlite3* m_db;
    };

    class MessageStorage : public SQLite3Storage
    {
    public:
         MessageStorage(const std::string&);
         ~MessageStorage();
         bool add(const andeme::schema::Message&); // возвращает true если это новое сообщение, false если уже есть
         std::vector<andeme::schema::Message> getAllMessages();// передаёт в callback по одному все сообщения
    };
int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName);

}

#endif // ANDEME_COMMON_SQLITE_UTIL_H
