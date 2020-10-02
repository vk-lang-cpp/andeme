#ifndef QTGUI_SETTINGS_H_
#define QTGUI_SETTINGS_H_
#include <QSettings>

namespace {
const char* SETTINGS_FILENAME = "settings.ini";
}

namespace andeme {
class Settings {
   public:
    Settings();
    Settings(std::string);
    ~Settings() { WriteSettings(); }
    void ReadSettings();
    void WriteSettings();
    void SetDefaultSettings();

    std::string Hostname();
    std::string Username();
    std::string Portname();

    void setHostname(const QString& value);
    void setPortname(const QString& value);
    void setUsername(const QString& value);

   private:
    QSettings settings_;
    QString hostname_;
    QString username_;
    QString portname_;
    bool modified_;
};
}  // namespace andeme

#endif  // QTGUI_SETTINGS_H_
