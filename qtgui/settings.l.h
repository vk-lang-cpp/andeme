#ifndef QTGUI_SETTINGS_H_
#define QTGUI_SETTINGS_H_
#include <QSettings>

// TODO: сделать ui для настройки
namespace andeme {
class Settings {
   public:
    Settings();
    Settings(char*);
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
};
}  // namespace andeme

#endif  // QTGUI_SETTINGS_H_
