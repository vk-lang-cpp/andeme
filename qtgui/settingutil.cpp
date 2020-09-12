#include "settingutil.h"
namespace andeme {
settingutil::settingutil():m_settings("settings.ini",QSettings::IniFormat)
{
    SetDefaultSettings();
    ReadSettings();
}

void settingutil::ReadSettings(){
    m_settings.beginGroup("/Settings");
    m_address  = m_settings.value("/Server","").toString();
    m_settings.endGroup();
}

void settingutil::WriteSettings(){
    m_settings.beginGroup("/Settings") ;
    m_settings.setValue("/Server",m_address);
    m_settings.endGroup();
}

void settingutil::SetDefaultSettings(){
    m_address = "localhost:12345";
}
}
