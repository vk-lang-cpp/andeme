#ifndef QTGUI_SETTINGDIALOG_H
#define QTGUI_SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    Ui::SettingDialog *ui;
};

#endif // QTGUI_SETTINGDIALOG_H
