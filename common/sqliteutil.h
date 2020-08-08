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
    using Callback = std::function<bool(const Row&)>;

    class SQLite3Common {
    protected:
        SQLite3Common();
        ~SQLite3Common();
        bool ExecuteQuery(const std::string& query, const Callback&);

        //copying is prohibited
        SQLite3Common(SQLite3Common const &) = delete;
        SQLite3Common& operator=(SQLite3Common const &) = delete;

        sqlite3* db_;
    };

    class MessageStorage : public SQLite3Common
    {
    public:
         MessageStorage(const std::string&);
         ~MessageStorage();
         bool AddMessage(const andeme::schema::Message&);
         std::vector<andeme::schema::Message> getAllMessages();
    };
int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName);

}

#endif // ANDEME_COMMON_SQLITE_UTIL_H
