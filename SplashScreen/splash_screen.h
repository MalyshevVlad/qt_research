#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <QMainWindow>
#include <QTimer>

#include "main_window.h"

namespace Ui {
class splash_screen;
}

class splash_screen : public QMainWindow
{
    Q_OBJECT

public:
    explicit splash_screen(QWidget *parent = nullptr);
    ~splash_screen();

private slots:
    void on_progress();

private:
    Ui::splash_screen *ui;
    QTimer* timer;
    quint32 counter = 0;
};

#endif // SPLASH_SCREEN_H
