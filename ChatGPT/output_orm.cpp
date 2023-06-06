#include "output_orm.h"
#include "ui_output_orm.h"

output_orm::output_orm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::output_orm)
{
    ui->setupUi(this);
}

output_orm::~output_orm()
{
    delete ui;
}
