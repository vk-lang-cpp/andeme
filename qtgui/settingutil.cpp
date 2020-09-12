#include "settingutil.h"

settingutil::settingutil():m_settings("settings.ini",QSettings::IniFormat)
{

    ReadSettings();
}

void settingutil::ReadSettings(){
    m_settings.beginGroup("/Settings");
    m_address  = m_settings.value("/Server","").toString();
    m_settings.endGroup();
}

void settingutil::WriteSettings(){
    m_settings.beginGroup("/Settings") ;
    m_settings.setValue("/text",m_address);
    m_settings.endGroup();
}

void settingutil::SetDefaultSettings(){
    m_address = "0.0.0.0:123456";
}
