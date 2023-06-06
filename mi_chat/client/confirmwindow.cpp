#include "confirmwindow.h"
#include "ui_confirmwindow.h"

ConfirmWindow::ConfirmWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmWindow)
{
    ui->setupUi(this);
}

ConfirmWindow::~ConfirmWindow()
{
    delete ui;
}
