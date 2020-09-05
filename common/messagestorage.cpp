#include "messagestorage.h"

namespace andeme {

MessageStorage::MessageStorage(const std::string &dbname)
    : SQLite3Storage(dbname) {
    const std::string sql =
        "CREATE table IF NOT EXISTS MESSAGES ("
        "Message TEXT NOT NULL)";

    ExecuteQuery(sql, nullptr);

    // preparing statements
    insertStmt_ = PrepareQuery(
        "INSERT INTO MESSAGES (Message)"
        "VALUES (@msg)");
    selectStmt_ = PrepareQuery("SELECT * FROM MESSAGES");
}

bool MessageStorage::AddMessage(const andeme::schema::Message &msg) {
    if (!insertStmt_) {
        return false;
    }

    insertStmt_.Reset();

    if (!insertStmt_.Bind("@msg", msg.text())) {
        return false;
    }

    return insertStmt_.Execute();
}

std::vector<andeme::schema::Message> MessageStorage::GetAllMessages() const {
    std::vector<andeme::schema::Message> messages;

    if (selectStmt_) {
        while (selectStmt_.Step()) {
            andeme::schema::Message msg;
            msg.set_text(selectStmt_.GetColumnText(0));
            messages.push_back(std::move(msg));
        }
        selectStmt_.Reset();
    }
    return messages;
}

}  // namespace andeme
