#include "auth_window.h"
#include "ui_auth_window.h"

//-------------------------------------------------------------------------------------------------
auth_window::auth_window(QWidget *parent) : QWidget(parent), ui(new Ui::auth_window)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
//-------------------------------------------------------------------------------------------------
auth_window::~auth_window() {
    delete ui;
}
//-------------------------------------------------------------------------------------------------
QString auth_window::get_name() {
    return auth_window::m_user_name;
}
//-------------------------------------------------------------------------------------------------
QString auth_window::get_password() {
    return auth_window::m_user_password;
}
//-------------------------------------------------------------------------------------------------
void auth_window::on_lineEdit_name_textEdited(const QString &arg) {
    auth_window::m_user_name = arg;
}
//-------------------------------------------------------------------------------------------------
void auth_window::on_lineEdit_password_textEdited(const QString &arg) {
    auth_window::m_user_password = arg;
}
//-------------------------------------------------------------------------------------------------
void auth_window::on_pushButton_sing_up_clicked() {
    emit sing_up_pushButton_clicked();
}
//-------------------------------------------------------------------------------------------------
void auth_window::on_pushButton_sing_in_clicked() {
    emit sing_in_pushButton_clicked();
}
//-------------------------------------------------------------------------------------------------
