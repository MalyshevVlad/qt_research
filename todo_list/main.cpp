#include "main_window.h"

#include <QApplication>
#include <QFile>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    main_window window;

    QFile file(":/qss/style/style.css");
    file.open(QFile::ReadOnly);
    window.setStyleSheet(file.readAll());

    window.show();
    return app.exec();
}
