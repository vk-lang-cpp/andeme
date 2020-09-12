#ifndef SETTINGUTIL_H
#define SETTINGUTIL_H
#include <QSettings>

//TODO: сделать ui для настройки

class settingutil
{
public:
    settingutil();
    ~settingutil(){WriteSettings();}
    void ReadSettings();
    void WriteSettings();
    void SetDefaultSettings();
private:
    QSettings m_settings;
    QString m_address;
};

#endif // SETTINGUTIL_H
