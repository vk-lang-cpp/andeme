#include "sqliteutil.h"
#include <chrono>
#include <ctime>
namespace  {
  class SqliteGuard
  {
    public:
      SqliteGuard() {
        sqlite3_initialize();
      }
        ~SqliteGuard() {
          sqlite3_shutdown();
      }
  };

  int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName){
    Callback& callback = *static_cast<Callback*>(NotUsed);

    Row row;

    for (size_t i = 0; i < argc; ++i)
      row.push_back(argv[i]);

    if (callback(row))
      return SQLITE_OK;
  }
}

namespace andeme {

  SQLite3Storage::SQLite3Storage(const std::string& dbname){
    static SqliteGuard guard;
    sqlite3_open_v2(dbname.data(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  }

  SQLite3Storage::~SQLite3Storage(){
    sqlite3_close_v2(db_);
  }

  bool SQLite3Storage::ExecuteQuery(const std::string& query,const Callback& callback){
    return (SQLITE_OK == sqlite3_exec(db_, query.c_str(), sqlite_callback, (void *)(&callback), nullptr));
  }

  Sqlite3Statement SQLite3Storage::PrepareQuery(const std::string& data){
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, data.data(), -1,&stmt, nullptr) == SQLITE_OK)
        return Sqlite3Statement(stmt);
    else
        return Sqlite3Statement();
  }

  MessageStorage::MessageStorage(const std::string& dbname): SQLite3Storage(dbname){
    const std::string sql = "CREATE table IF NOT EXISTS MESSAGES ("
                            "Message TEXT NOT NULL);";

    ExecuteQuery(sql,nullptr);

    //preparing statements
    insertStmt_ = PrepareQuery("INSERT INTO MESSAGES ('Message')"
                                 "VALUES (@msg);");
    selectStmt_ = PrepareQuery("SELECT * FROM 'MESSAGES';");
  }
  bool MessageStorage::AddMessage(const andeme::schema::Message& msg){
    if (insertStmt_.isValid()){
      if (!insertStmt_.Bind("@msg",msg.text())){
        return false;
      }
      if (insertStmt_.Execute()!= SQLITE_DONE){
        return false;
      }
      insertStmt_.Reset();
      return true;
    }
    else
        return false;
  }

  std::vector<andeme::schema::Message> MessageStorage::getAllMessages(){
    if (selectStmt_.isValid()){
      std::vector<andeme::schema::Message> messages;
      while (selectStmt_.Execute() == SQLITE_ROW) {
        andeme::schema::Message msg;
        msg.set_text(selectStmt_.GetColumnText(0));
        messages.push_back(std::move(msg));
      }
      selectStmt_.Reset();
      return messages;
    }
    std::vector<andeme::schema::Message>();
  }

}
