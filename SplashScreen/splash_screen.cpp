#include "splash_screen.h"
#include "main_window.h"
#include "ui_splash_screen.h"

#include <QGraphicsDropShadowEffect>



splash_screen::splash_screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::splash_screen),
    timer(new QTimer())
{
    ui->setupUi(this);

    // REMOVE TITLE BAR
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);


    // DROP SHADOW EFFECT
    QGraphicsDropShadowEffect shadow;
    shadow.setBlurRadius(20);
    shadow.setXOffset(0);
    shadow.setXOffset(0);
    shadow.setColor(QColor(0, 0, 0, 60));
    ui->frame_drop_shadow->setGraphicsEffect(&shadow);

    // QTIMER ==> START

    connect(timer, SIGNAL(timeout()), this, SLOT(on_progress()));
    timer->start(35);

}

splash_screen::~splash_screen()
{
    delete ui;
}

void splash_screen::on_progress()
{
    ui->progressBar->setValue(counter);

    if (counter > 100) {
        timer->stop();
        this->close();

        main_window main;
        main.show();
    }
    counter++;
}
