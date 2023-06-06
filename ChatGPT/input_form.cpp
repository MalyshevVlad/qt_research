#include "input_form.h"
#include "ui_input_form.h"

input_form::input_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_form)
{
    ui->setupUi(this);
}

input_form::~input_form()
{
    delete ui;
}
