#ifndef SETTINGUTIL_H
#define SETTINGUTIL_H
#include <QSettings>

//TODO: сделать ui для настройки
namespace andeme {
class settingutil{
public:
    settingutil();
    ~settingutil(){WriteSettings();}
    void ReadSettings();
    void WriteSettings();
    void SetDefaultSettings();
    std::string getHostname();
    std::string getUsername();
private:
    QSettings m_settings;
    QString m_hostname;
    QString m_username;
};
}

#endif // SETTINGUTIL_H
