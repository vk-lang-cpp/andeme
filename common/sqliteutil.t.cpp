#include <gtest/gtest.h>
#include <memory>
#include "sqlite3.h"
#include "sqliteutil.h"
#include <algorithm>
#include <filesystem>

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

namespace {
namespace fs = std::filesystem;

struct TempDirectory {
   std::filesystem::path dirname;
   TempDirectory() : dirname(std::filesystem::temp_directory_path() / "AndemeTmp") {
        std::filesystem::create_directories(dirname);
    }
    ~TempDirectory() {
        std::filesystem::remove_all(dirname);
    }
};
} // close namespace


TEST(Sqlite3DataBase, MessageAddTest) {//добавление одного сообщения
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage db( (tmpfolder.dirname / "testDatabase.db").string());
        EXPECT_TRUE(fs::exists(tmpfolder.dirname / "testDatabase.db")); //проверка наличия файла

        andeme::schema::Message message;
        message.set_text("test_message");
        EXPECT_EQ(true, db.AddMessage(message));
    }
}

TEST(Sqlite3DataBase,ReadMessageTest){ //проверка на запись/чтение вектора строк
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage db( (tmpfolder.dirname / "testDatabase.db").string());

        EXPECT_TRUE(fs::exists(tmpfolder.dirname / "testDatabase.db"));

        std::vector<std::string> TestMessages  =
                {"Is inquiry no he several excited am.",
                 "undefined. Celebrated delightful",
                 "an especially increasing",
                 "instrument am. He felicity no an at",
                 "packages answered opinions juvenile.",
                 "Course sir people worthy",
                 "horses add entire",
                 "suffer. Indulgence contrasted sufficient to",
                 "unpleasant in in insensible",
                 "favourable. Hard do me si"};

        std::vector<andeme::schema::Message> testInbox(TestMessages.size());

        //задаем текст входящих сообщений объектам Message и заполняем ими базу
        for (size_t i = 0; i < TestMessages.size(); ++i) {
            testInbox[i].set_text(TestMessages[i].data());
            EXPECT_EQ(true, db.AddMessage(testInbox[i]));
        }

        //получаем все значения из базы
        std::vector<andeme::schema::Message> testOutbox = db.getAllMessages();

        //сравниваем кол-во писем
        EXPECT_EQ(testInbox.size(),testOutbox.size());

        //сравниваем текст сообщений,сортировка по ID
          for (size_t i = 0; i < TestMessages.size(); i++) {
              EXPECT_STREQ(testInbox[i].text().c_str(), testOutbox[i].text().c_str());
          }
    }
}

TEST(Sqlite3DataBase_v2, MessageAddTest) {//добавление одного сообщения
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage_v2 db( (tmpfolder.dirname / "testDatabase.db").string());
        EXPECT_TRUE(fs::exists(tmpfolder.dirname / "testDatabase.db")); //проверка наличия файла

        andeme::schema::Message message;
        message.set_text("test_message");
        EXPECT_EQ(true, db.AddMessage(message));
    }
}

TEST(Sqlite3DataBase_v2,ReadMessageTest){ //проверка на запись/чтение вектора строк
    {
        TempDirectory tmpfolder;
        andeme::MessageStorage_v2 db( (tmpfolder.dirname / "testDatabase.db").string());

        EXPECT_TRUE(fs::exists(tmpfolder.dirname / "testDatabase.db"));

        std::vector<std::string> TestMessages  = {"one","two","three","four","five","six","seven","eight"};
        std::vector<andeme::schema::Message> testInbox(TestMessages.size());

        //задаем текст входящих сообщений объектам Message и заполняем ими базу
        for (size_t i = 0; i < TestMessages.size(); ++i) {
            testInbox[i].set_text(TestMessages[i].data());
            EXPECT_EQ(true, db.AddMessage(testInbox[i]));
        }

        //получаем все значения из базы
        std::vector<andeme::schema::Message> testOutbox = db.getAllMessages();
        //сравниваем кол-во писем
        EXPECT_EQ(testInbox.size(),testOutbox.size());

        //сравниваем текст сообщений,сортировка по ID
          for (size_t i = 0; i < TestMessages.size(); i++) {
              EXPECT_STREQ(testInbox[i].text().c_str(), testOutbox[i].text().c_str());

          }
    }
}

