#include "choicecreate.h"
#include "ui_choicecreate.h"

ChoiceCreate::ChoiceCreate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoiceCreate)
{
    ui->setupUi(this);
}

ChoiceCreate::~ChoiceCreate()
{
    delete ui;
}
