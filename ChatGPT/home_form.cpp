#include "home_form.h"
#include "ui_home_form.h"

home_form::home_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::home_form)
{
    ui->setupUi(this);
}

home_form::~home_form()
{
    delete ui;
}
