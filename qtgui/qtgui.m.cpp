#include <messagemanager.h>
#include <settings.h>
#include <ui_qtgui.h>
#include <QApplication>
#include <QObject>
#include <QPushButton>
#include <iostream>
//спрятать подальше,т.к создаватся окно будет динамически
#include <settingdialog.h>

int main(int argc, char** argv) {
    andeme::Settings settings;
    QApplication app(argc, argv);
    QMainWindow widget;
    Ui::MainWindow ui;
    ui.setupUi(&widget);
    widget.show();

    MessageManager m(
        (settings.Hostname()+":"+settings.Portname()),
        [&ui]() -> std::string {
            std::string msg = ui.inputMessage->toPlainText().toStdString();
            ui.inputMessage->clear();
            return msg;
        },
        [&ui](const std::string& msg) -> void {
            ui.outputMessages->append(msg.c_str());
        });


    QObject::connect(ui.settingButton,&QPushButton::clicked,[](){
        //добавить предка
        static SettingWindow* ad = new SettingWindow;
        ad->show();

    });
    QObject::connect(ui.sendMessage, SIGNAL(released()), &m, SLOT(send()));

    return app.exec();
}
