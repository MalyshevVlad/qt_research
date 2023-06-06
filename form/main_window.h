#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtSql/QtSql>
#include "auth_window.h"
#include "reg_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class main_window; }
QT_END_NAMESPACE

class main_window : public QMainWindow {
    Q_OBJECT
private:
    Ui::main_window *ui_main;

    auth_window ui_auth; // экземпляры окна авторизации и окна регистрации
    reg_window ui_reg; // принадлежат главному окну

    QString m_user_name;
    QString m_user_email;
    QString m_user_password;

    int m_user_counter;
    bool m_loginSuccesfull;

    QString db_input;
    QSqlDatabase mw_db; // подключения к БД

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();
    void display();
    bool connectDB();

private slots:
    void on_authorize_user();
    void on_register_user();
    void on_authorize_window_show();
    void on_register_window_show();

};
#endif // MAIN_WINDOW_H
