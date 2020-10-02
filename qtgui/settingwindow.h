#ifndef QTGUI_SETTINGDIALOG_H
#define QTGUI_SETTINGDIALOG_H
#include <QDialog>
#include "settings.l.h"

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog {
    Q_OBJECT
    andeme::Settings settings;

   public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

   private:
    Ui::SettingWindow *ui;
};

#endif  // SETTINGDIALOG_H

