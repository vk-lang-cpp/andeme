#include <gtest/gtest.h>
#include <memory>
#include "sqlite3.h"
#include "sqliteutil.h"
#include <algorithm>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

TEST(Sqlite3DataBase, MessageAddTest) {//добавление одного сообщения

    andeme::MessageStorage db("testDatabase1.db");
    andeme::schema::Message message;
    message.set_text("test_message");
    EXPECT_EQ(true, db.add(message));

}

TEST(Sqlite3DataBase,ReadMessageTest){ //проверка на запись/чтение вектора строк

    andeme::MessageStorage db2("testDatabase2.db");
    std::vector<std::string> TestMessages  = {"1one","2two","3three","4four","5five","6six","7seven","8eight"};
    std::vector<andeme::schema::Message> testInbox(TestMessages.size());

    for (size_t i = 0; i < TestMessages.size(); ++i) {
        testInbox[i].set_text(TestMessages[i].data());
        EXPECT_EQ(true, db2.add(testInbox[i]));
    }

    std::vector<andeme::schema::Message> testOutbox = db2.getAllMessages();

      for (size_t i = 0; i < TestMessages.size(); i++) {
          EXPECT_STREQ(testInbox[i].text().c_str(), testOutbox[i].text().c_str());
      }
}
