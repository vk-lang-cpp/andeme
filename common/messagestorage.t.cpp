#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>

#include "messagestorage.h"

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

namespace {
namespace fs = std::filesystem;

struct TempDirectory {
    std::filesystem::path dirname;
    TempDirectory()
        : dirname(std::filesystem::temp_directory_path() / "AndemeTmp") {
        std::filesystem::create_directories(dirname);
    }
    ~TempDirectory() { std::filesystem::remove_all(dirname); }
};
}  // namespace

TEST(MessageStorage, MessageAddTest) {  //добавление одного сообщения
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage db(
            (tmpfolder.dirname / "testDatabase.db").string());
        EXPECT_TRUE(fs::exists(tmpfolder.dirname /
                               "testDatabase.db"));  //проверка наличия файла

        andeme::schema::Message message;
        message.mutable_msg()->set_text("test_message");
        ASSERT_TRUE(db.AddMessage(message));
    }
}

TEST(MessageStorage,
     ReadMessageTest) {  //проверка на запись/чтение вектора строк
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage db(
            (tmpfolder.dirname / "testDatabase.db").string());

        ASSERT_TRUE(fs::exists(tmpfolder.dirname / "testDatabase.db"));

        std::vector<std::string> TestMessages = {
            "one", "two", "three", "four", "five", "six", "seven", "eight"};
        std::vector<andeme::schema::Message> testInbox(TestMessages.size());

        //задаем текст входящих сообщений объектам Message и заполняем ими базу
        for (size_t i = 0; i < TestMessages.size(); ++i) {
            testInbox[i].mutable_msg()->set_text(TestMessages[i]);
            ASSERT_TRUE(db.AddMessage(testInbox[i]));
        }

        //получаем все значения из базы
        std::vector<andeme::schema::Message> testOutbox = db.GetAllMessages();
        //сравниваем кол-во писем
        ASSERT_EQ(testInbox.size(), testOutbox.size());

        //сравниваем текст сообщений,сортировка по ID
        for (size_t i = 0; i < TestMessages.size(); i++) {
            ASSERT_EQ(testInbox[i].signedMessage_case(),testInbox[i].kMsg);
            EXPECT_STREQ(testInbox[i].mutable_msg()->text().c_str(),
                         testOutbox[i].mutable_msg()->text().c_str());
        }
    }
}
