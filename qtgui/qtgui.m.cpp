#include <QApplication>
#include <ui_qtgui.h>
#include <iostream>
#include <QObject>
#include <messagemanager.h>
#include <settingutil.h>


int main(int argc, char **argv) {
    andeme::settingutil settings;
    QApplication app(argc, argv);
    QMainWindow widget;
    Ui::MainWindow ui;
    ui.setupUi(&widget);
    widget.show();

    MessageManager m(
        settings.getHostname(),
        [&ui]() -> std::string { 
            std::string msg = ui.inputMessage->toPlainText().toStdString();
            ui.inputMessage->clear();
            return msg;
        },
        [&ui](const std::string& msg) -> void {
            ui.outputMessages->append(msg.c_str());
        }
    );

    QObject::connect(
        ui.sendMessage, SIGNAL(released()),
        &m, SLOT(send()));

    return app.exec();
}
