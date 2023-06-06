#include "output_form.h"
#include "ui_output_form.h"

output_form::output_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::output_form)
{
    ui->setupUi(this);
}

output_form::~output_form()
{
    delete ui;
}
