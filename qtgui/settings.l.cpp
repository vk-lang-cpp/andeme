#include "settings.l.h"
#include <QDebug>
#include <QtGui>
namespace andeme {

Settings::Settings()
    : settings_(SETTINGS_FILENAME, QSettings::IniFormat), modified_(false) {
    //при первом запуске,значения не записываются в файл до вызова метода
    // writeSettings,поэтому создан флаг modified_ для проверки наличия
    //записей,чтобы не перезаписать их.
    ReadSettings();
    if (modified_ == false) {
        SetDefaultSettings();
        WriteSettings();
    }
}

Settings::Settings(std::string path)
    : settings_(path.data(), QSettings::IniFormat), modified_(false) {
    ReadSettings();
    if (modified_ == false) {
        SetDefaultSettings();
        WriteSettings();
    }
}

void Settings::ReadSettings() {
    settings_.beginGroup("/Settings");
    hostname_ = settings_.value("/Hostname").toString();
    username_ = settings_.value("/Username").toString();
    portname_ = settings_.value("/Portname").toString();
    settings_.endGroup();
}

void Settings::WriteSettings() {
    modified_ = true;
    settings_.beginGroup("/Settings");
    settings_.setValue("/Hostname", hostname_);
    settings_.setValue("/Username", username_);
    settings_.setValue("/Portname", portname_);
    settings_.setValue("/modified", modified_);
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
