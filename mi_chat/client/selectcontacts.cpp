#include "selectcontacts.h"
#include "ui_selectcontacts.h"

SelectContacts::SelectContacts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectContacts)
{
    ui->setupUi(this);
}

SelectContacts::~SelectContacts()
{
    delete ui;
}
