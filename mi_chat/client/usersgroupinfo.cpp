#include "usersgroupinfo.h"
#include "ui_usersgroupinfo.h"

UsersGroupInfo::UsersGroupInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersGroupInfo)
{
    ui->setupUi(this);
}

UsersGroupInfo::~UsersGroupInfo()
{
    delete ui;
}
