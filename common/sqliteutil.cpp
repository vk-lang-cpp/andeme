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
}

namespace andeme {

    SQLite3Common::SQLite3Common(){
        static SqliteGuard guard;
    }

    SQLite3Common::~SQLite3Common(){
        sqlite3_close_v2(db_);
    }

    bool SQLite3Common::ExecuteQuery(const std::string& query,const Callback& callback){
        return (SQLITE_OK == sqlite3_exec(db_, query.c_str(), sqlite_callback, (void *)(&callback), nullptr));
    }

    bool SQLite3Common::PrepareQuery(Sqlite3Statement& stmt,const std::string& data){
        return (sqlite3_prepare_v2(db_, data.data(), -1,&stmt.stmt_, nullptr) == SQLITE_OK);
    }

    MessageStorage::MessageStorage(const std::string & dbname){

        const std::string sql = "CREATE table IF NOT EXISTS MESSAGES ("
                                "ID INTEGER PRIMARY KEY,"
                                "Timestamp INTEGER NOT NULL,"
                                "Author TEXT NOT NULL,"
                                "Message TEXT NOT NULL,"
                                "Signature TEXT NOT NULL ,UNIQUE(Timestamp,Message));";

        sqlite3_open_v2(dbname.data(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        ExecuteQuery(sql.data(),nullptr);
    }
    bool MessageStorage::AddMessage(const andeme::schema::Message & msg){
        std::string Author = "Author";
        std::string sign = "sign";
        std::string sql =  "INSERT INTO MESSAGES ('Timestamp', 'Author', 'Message','Signature')"
                           "VALUES (datetime('now','localtime'),'"+ Author +"','"+ msg.text() +"','"+ sign +"');";

        return (ExecuteQuery(sql.data(),nullptr));
    }

    std::vector<andeme::schema::Message> MessageStorage::getAllMessages(){
        std::vector<andeme::schema::Message> messages;

        ExecuteQuery("SELECT Message FROM 'MESSAGES' ORDER BY ID ASC",[&messages](const Row& row)->bool{
           andeme::schema::Message msg;
           msg.set_text(row[0]);
           messages.push_back(std::move(msg));
           return true;});
        return messages;
    }

    MessageStorage_v2::MessageStorage_v2(const std::string& dbname){
        const std::string sql = "CREATE table IF NOT EXISTS MESSAGES ("
                                "ID INTEGER PRIMARY KEY,"
                                "Timestamp INTEGER NOT NULL,"
                                "Author TEXT NOT NULL,"
                                "Message TEXT NOT NULL,"
                                "Signature TEXT NOT NULL ,UNIQUE(Timestamp,Message));";

         sqlite3_open_v2(dbname.data(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
         ExecuteQuery(sql.data(),nullptr);
    }
    bool MessageStorage_v2::AddMessage(const andeme::schema::Message& msg){
        Sqlite3Statement stmt;
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        time_t now_t = std::chrono::system_clock::to_time_t(now);
        std::string Author = "Author";
        std::string sign = "sign";
        std::string sql = "INSERT INTO MESSAGES ('Timestamp','Author', 'Message','Signature')"
                          "VALUES (@time,@author,@msg,@sign);";
    if(!PrepareQuery(stmt,sql)){
        return false;
      }

      if (!stmt.Bind("@time",now_t)){
         return false;
      }

      if (!stmt.Bind("@author",Author.data())){
         return false;
      }

      if (!stmt.Bind("@msg",msg.text().data())){
         return false;
      }

      if (!stmt.Bind("@sign",sign.data())){
          return false;
      }
      if (stmt.Execute()!= SQLITE_DONE){return false;}
         return true;
      }
    std::vector<andeme::schema::Message> MessageStorage_v2::getAllMessages(){
       Sqlite3Statement stmt;
       std::vector<andeme::schema::Message> messages;

       std::string sql = "SELECT * FROM 'MESSAGES' ORDER BY ID ASC;";

       if (!PrepareQuery(stmt,sql)){
           return std::vector<andeme::schema::Message>();
       }

       while (stmt.Execute() == SQLITE_ROW) {
           andeme::schema::Message msg;
           msg.set_text(stmt.GetColumnText(3));
           messages.push_back(std::move(msg));
        }
       return messages;
    }

   static int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
      andeme::Callback& callback = *static_cast<Callback*>(NotUsed);

      Row row;

      for (size_t i = 0; i < argc; ++i) {
          row.push_back(argv[i]);
      }
      if (callback(row))
         return SQLITE_OK;
    }
}
