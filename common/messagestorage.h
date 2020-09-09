#ifndef ANDEME_COMMON_MESSAGESTORAGE_H_
#define ANDEME_COMMON_MESSAGESTORAGE_H_
#include <sqlite3storage.h>

#include <schema/message.pb.h>
#include <vector>

namespace andeme {

class MessageStorage : public SQLite3Storage {
   public:
    MessageStorage(const std::string &);
    bool AddMessage(andeme::schema::Message &);
    std::vector<andeme::schema::Message> GetAllMessages() const;

   private:
    mutable Sqlite3Statement insertStmt_;
    mutable Sqlite3Statement selectStmt_;
};

}  // namespace andeme

#endif  // ANDEME_COMMON_MESSAGESTORAGE_H_
