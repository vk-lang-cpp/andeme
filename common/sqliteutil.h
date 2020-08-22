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

  class Sqlite3Statement;
  class SQLite3Common
  {
   protected:
     SQLite3Common();
     ~SQLite3Common();
     bool ExecuteQuery(const std::string& query, const Callback&);
     bool PrepareQuery(Sqlite3Statement& ,const std::string&);

     //copying is prohibited
     SQLite3Common(SQLite3Common const &) = delete;
     SQLite3Common& operator=(SQLite3Common const &) = delete;

     sqlite3* db_;
   };

   class MessageStorage : public SQLite3Common
   {
    public:
       MessageStorage(const std::string&);
       ~MessageStorage(){}
       bool AddMessage(const andeme::schema::Message&);
       std::vector<andeme::schema::Message> getAllMessages();
    };
  int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName);

  class MessageStorage_v2 : public SQLite3Common
  {
    public:
      MessageStorage_v2(const std::string&);
      ~MessageStorage_v2(){}
      bool AddMessage(const andeme::schema::Message&);
      std::vector<andeme::schema::Message> getAllMessages();
  };

  class Sqlite3Statement
  {
   public:
     Sqlite3Statement() = default;

     Sqlite3Statement(sqlite3_stmt *stmt)
        : stmt_(stmt) { sqlite3_finalize(stmt); }

     ~Sqlite3Statement() {
       sqlite3_finalize(stmt_); }

     bool Bind(int index, const std::string &value) {
         return 0 == sqlite3_bind_text(stmt_, index, value.data(),
                                      value.size(), nullptr);
     }

     bool Bind(int index, const int &value) {
         return 0 == sqlite3_bind_int(stmt_, index, value);
     }

     template<typename T>
     bool Bind(const std::string &name, const T &value) {
        int index = sqlite3_bind_parameter_index(stmt_, name.data());
        if (index == 0) {
           return false;
        }
        return Bind(index, value);
      }

    int Execute() {
      return  sqlite3_step(stmt_);
    }
    char* GetColumnText(int column) { return (char*)sqlite3_column_text(stmt_,column); }
    int GetColumnInt(int column) { return sqlite3_column_int(stmt_,column); }



    void Reset() { sqlite3_reset(stmt_); }

    friend class SQLite3Common;

  private:
    inline sqlite3_stmt *Stmt() { return stmt_; }
    sqlite3_stmt* stmt_;
  };
}
#endif // ANDEME_COMMON_SQLITE_UTIL_H
