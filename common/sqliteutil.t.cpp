#include <gtest/gtest.h>
#include <memory>
#include "sqlite3.h"
#include "sqliteutil.h"


TEST(SQLITE3BASE, MessageAddTest) {

    andeme::MessageStorage db("testbase.db");

    //добавление сообщения
    andeme::schema::Message message;
    message.set_text("test_message");
    EXPECT_EQ(true, db.add(message));



}
