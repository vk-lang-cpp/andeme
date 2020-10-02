#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>

#include "settings.l.h"

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

namespace {
namespace fs = std::filesystem;

struct TempDirectory {
    std::filesystem::path dirname;
    TempDirectory() : dirname(std::filesystem::current_path() / "AndemeTmp") {
        std::filesystem::create_directories(dirname);
    }
    ~TempDirectory() { std::filesystem::remove_all(dirname); }
};
}  // namespace

TEST(Settings, RawReadTest) {
    {
        TempDirectory tmpfolder;
        //чтение первоначальных настроек,чтение данных по умолчанию (три
        //значения)

        //каким-то образом,QSettings не хочет ничего создавать, если адрес
        //совпадает с /Local/Temp,поэтому я решил создать файл прямо на месте
        andeme::Settings settingfile(
            (tmpfolder.dirname / "testSettingFile.ini").string());

        EXPECT_STREQ("localhost", settingfile.Hostname().c_str());
        EXPECT_STREQ("12345", settingfile.Portname().c_str());
        EXPECT_STREQ("username", settingfile.Username().c_str());
    }
}

TEST(Settings, WriteReadTest) {
    {
        TempDirectory tmpfolder;
        // чтение/запись

        andeme::Settings settingfile(
            (tmpfolder.dirname / "testSettingFile.ini").string());

        settingfile.setHostname("test.ip.ru");
        settingfile.setPortname("5001");
        settingfile.setUsername("newbuy");

        settingfile.WriteSettings();

        EXPECT_STREQ("test.ip.ru", settingfile.Hostname().c_str());
        EXPECT_STREQ("5001", settingfile.Portname().c_str());
        EXPECT_STREQ("newbuy", settingfile.Username().c_str());

        settingfile.SetDefaultSettings();

        EXPECT_STREQ("localhost", settingfile.Hostname().c_str());
        EXPECT_STREQ("12345", settingfile.Portname().c_str());
        EXPECT_STREQ("username", settingfile.Username().c_str());
    }
}
