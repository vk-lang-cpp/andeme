#include "sqlite3storage.h"

namespace {

class SqliteGuard {
   public:
    SqliteGuard() { sqlite3_initialize(); }
    ~SqliteGuard() { sqlite3_shutdown(); }
};  // namespace

int sqlite_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    Callback& callback = *static_cast<Callback*>(NotUsed);

    Row row;

    for (int i = 0; i < argc; ++i) row.push_back(argv[i]);

    if (callback(row))
        return SQLITE_OK;
    else
        return SQLITE_ERROR;
}
}  // unnamed namespace

namespace andeme {

Sqlite3Statement::Sqlite3Statement(sqlite3_stmt* stmt)
    : stmt_(stmt, [](sqlite3_stmt* stmt) { sqlite3_finalize(stmt); }){};

bool Sqlite3Statement::Bind(int index, const std::string& value) {
    return SQLITE_OK == sqlite3_bind_text(stmt_.get(), index, value.data(),
                                          value.size(), nullptr);
}

bool Sqlite3Statement::Bind(int index, int value) {
    return SQLITE_OK == sqlite3_bind_int(stmt_.get(), index, value);
}

bool Sqlite3Statement::Execute() {
    return SQLITE_DONE == sqlite3_step(stmt_.get());
}

bool Sqlite3Statement::Step() {
    return SQLITE_ROW == sqlite3_step(stmt_.get());
}

const char* Sqlite3Statement::GetColumnText(int column) const {
    return reinterpret_cast<const char*>(
        sqlite3_column_text(stmt_.get(), column));
}

int Sqlite3Statement::GetColumnInt(int column) const {
    return sqlite3_column_int(stmt_.get(), column);
}

void Sqlite3Statement::Reset() { sqlite3_reset(stmt_.get()); }

SQLite3Storage::SQLite3Storage(const std::string& dbname) {
    static SqliteGuard guard;
    if (sqlite3_open_v2(dbname.data(), &db_,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                        nullptr) != SQLITE_OK)
        throw std::runtime_error(std::string(sqlite3_errmsg(db_)));
}

SQLite3Storage::~SQLite3Storage() { sqlite3_close_v2(db_); }

bool SQLite3Storage::ExecuteQuery(const std::string& query,
                                  const Callback& callback) {
    return (SQLITE_OK == sqlite3_exec(db_, query.c_str(), sqlite_callback,
                                      (void*)(&callback), nullptr));
}

Sqlite3Statement SQLite3Storage::PrepareQuery(const std::string& data) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, data.data(), -1, &stmt, nullptr) == SQLITE_OK)
        return Sqlite3Statement(stmt);
    else
        throw std::runtime_error(std::string(sqlite3_errmsg(db_)));
}

}  // namespace andeme
