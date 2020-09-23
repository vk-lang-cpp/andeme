#include "settings.h"

namespace {
const char* SETTINGS_FILENAME = "settings.ini";
}

namespace andeme {

Settings::Settings() : settings_(SETTINGS_FILENAME, QSettings::IniFormat) {
    SetDefaultSettings();
    ReadSettings();
}

void Settings::ReadSettings() {
    settings_.beginGroup("/Settings");
    hostname_ = settings_.value("/Hostname", "").toString();
    username_ = settings_.value("/Username", "").toString();
    portname_ = settings_.value("/Portname", "").toString();
    settings_.endGroup();
}

void Settings::WriteSettings() {
    settings_.beginGroup("/Settings");
    settings_.setValue("/Hostname", hostname_);
    settings_.setValue("/Username", username_);
    settings_.setValue("/Portname", portname_);
    settings_.endGroup();
}

void Settings::SetDefaultSettings() {
    hostname_ = "localhost";
    portname_ = "12345";
    username_ = "username";
}
std::string Settings::Hostname() { return hostname_.toStdString(); }
std::string Settings::Username() { return username_.toStdString(); }

std::string Settings::Portname() { return portname_.toStdString(); }

void Settings::setHostname(const QString& value) { hostname_ = value; }

void Settings::setPortname(const QString& value) { portname_ = value; }

void Settings::setUsername(const QString& value) { username_ = value; }

}  // namespace andeme
