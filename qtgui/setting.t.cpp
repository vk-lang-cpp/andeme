#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>

#include "settings.h"

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

TEST(Settings, settingTest) {
  {     TempDirectory tmpfolder;
        andeme::Settings settingfile;

  }
}

