#include "reg_window.h"
#include "ui_reg_window.h"
//-------------------------------------------------------------------------------------------------
reg_window::reg_window(QWidget *parent) : QWidget(parent), ui(new Ui::reg_window)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
//-------------------------------------------------------------------------------------------------
reg_window::~reg_window() {
    delete ui;
}
//-------------------------------------------------------------------------------------------------
QString reg_window::get_name() {
    return m_user_name;
}
//-------------------------------------------------------------------------------------------------
QString reg_window::get_email() {
    return m_user_email;
}
//-------------------------------------------------------------------------------------------------
QString reg_window::get_password() {
    return m_user_password;
}
//-------------------------------------------------------------------------------------------------
bool check_name(const QString& arg) {

}
//-------------------------------------------------------------------------------------------------
bool check_email(const QString& arg) {

}
//-------------------------------------------------------------------------------------------------
bool check_password(const QString& arg) {

}
//-------------------------------------------------------------------------------------------------
void reg_window::on_lineEdit_name_textEdited(const QString &arg) {
    reg_window::m_user_name = arg;
}
//-------------------------------------------------------------------------------------------------
void reg_window::on_lineEdit_email_textEdited(const QString &arg) {
    reg_window::m_user_email = arg;
}
//-------------------------------------------------------------------------------------------------
void reg_window::on_lineEdit_password_textEdited(const QString &arg) {
    reg_window::m_user_password = arg;
}
//-------------------------------------------------------------------------------------------------
void reg_window::on_pushButton_sing_up_clicked() {
    emit sing_up_pushButton_clicked();
}
//-------------------------------------------------------------------------------------------------
void reg_window::on_pushButton_sing_in_clicked() {
    emit sing_in_pushButton_clicked();
}
//-------------------------------------------------------------------------------------------------
