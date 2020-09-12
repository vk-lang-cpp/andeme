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
    std::string getaddress(){return m_address.toStdString();}
private:
    QSettings m_settings;
    QString m_address;
};
}

#endif // SETTINGUTIL_H
