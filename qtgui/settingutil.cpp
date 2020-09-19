#include "settingutil.h"
namespace andeme {

settingutil::settingutil():m_settings("settings.ini",QSettings::IniFormat)
{
    SetDefaultSettings();
    ReadSettings();
}

void settingutil::ReadSettings(){
    m_settings.beginGroup("/Settings");
    m_hostname  = m_settings.value("/Hostname","").toString();
    m_username = m_settings.value("/Username","").toString();
    m_settings.endGroup();
}

void settingutil::WriteSettings(){
    m_settings.beginGroup("/Settings") ;
    m_settings.setValue("/Hostname",m_hostname);
    m_settings.setValue("/Username",m_username);
    m_settings.endGroup();
}

void settingutil::SetDefaultSettings(){
    m_hostname = "localhost:12345";
}
std::string settingutil::getHostname(){
    return m_hostname.toStdString();
}
std::string settingutil::getUsername(){
    return m_username.toStdString();
}
}
