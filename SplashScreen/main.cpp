#include "main_window.h"
#include "splash_screen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    splash_screen s;
    s.show();
    return a.exec();
}
