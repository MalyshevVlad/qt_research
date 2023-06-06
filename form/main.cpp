#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    main_window w;
    w.display();
    return a.exec();
}
