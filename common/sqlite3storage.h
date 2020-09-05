#ifndef ANDEME_COMMON_SQLITE3STORAGE_H_
#define ANDEME_COMMON_SQLITE3STORAGE_H_
#include <sqlite3.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
using Row = std::vector<std::string>;
using Callback = std::function<bool(const Row &)>;
int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName);
}  // unnamed namespace

namespace andeme {

class Sqlite3Statement {
   public:
    Sqlite3Statement() = default;

    explicit operator bool() const noexcept;

    bool Bind(int index, const std::string &value);
    bool Bind(int index, int value);

    template <typename T>
    bool Bind(const std::string &name, const T &value) {
        int index = sqlite3_bind_parameter_index(stmt_.get(), name.data());
        if (index == 0) {
            return false;
        }
        return Bind(index, value);
    }

    bool Execute();
    bool Step();
    const char *GetColumnText(int column) const;
    int GetColumnInt(int column) const;
    void Reset();

    friend class SQLite3Storage;

   private:
    Sqlite3Statement(sqlite3_stmt *stmt);
    const sqlite3_stmt *Stmt() const;

    std::shared_ptr<sqlite3_stmt> stmt_;
};

class SQLite3Storage {
   protected:
    SQLite3Storage(const std::string &dbname);
    ~SQLite3Storage();

    bool ExecuteQuery(const std::string &query, const Callback &callback);
    Sqlite3Statement PrepareQuery(const std::string &sql);

    // copying is prohibited
    SQLite3Storage(SQLite3Storage const &) = delete;
    SQLite3Storage &operator=(SQLite3Storage const &) = delete;

   private:
    sqlite3 *db_;
};

inline Sqlite3Statement::operator bool() const noexcept {
    return static_cast<bool>(stmt_);
}
inline const sqlite3_stmt *Sqlite3Statement::Stmt() const {
    return stmt_.get();
}

}  // namespace andeme

#endif  // ANDEME_COMMON_SQLITE3STORAGE_H_
