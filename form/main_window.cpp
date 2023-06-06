#include <QDebug>

#include "main_window.h"
#include "ui_main_window.h"
#include "auth_window.h"
#include "reg_window.h"

//-------------------------------------------------------------------------------------------------
main_window::main_window(QWidget *parent) :QMainWindow(parent), ui_main(new Ui::main_window) {

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_user_counter = 0;
    m_loginSuccesfull = false;

    connect(&ui_auth, SIGNAL(sing_in_pushButton_clicked()), this, SLOT(on_authorize_user()));
    connect(&ui_auth, SIGNAL(sing_up_pushButton_clicked()), this, SLOT(on_register_window_show()));
    connect(&ui_auth, SIGNAL(destroyed()), this, SLOT(show()));

    connect(&ui_reg, SIGNAL(sing_up_pushButton_clicked()), this, SLOT(on_register_user()));
    connect(&ui_reg, SIGNAL(sing_in_pushButton_clicked()), this, SLOT(on_authorize_window_show()));
    connect(&ui_reg, SIGNAL(destroyed()), &ui_auth, SLOT(show()));

    if(!connectDB()) {
        qDebug() << "Failed to connect DB";
    }

//    QSqlQuery query;

//    db_input = "CREATE TABLE userlist"
//                "( "
//                    "id INTEGER PRIMARY KEY NOT NULL,"
//                    "name VARCHAR(30), "
//                    "email VARCHAR(320), "
//                    "password VARCHAR(12) "
//                ");";

//    if(!query.exec(db_input)) {
//        qDebug() << "Unable to create a table" << query.lastError();
//    }

    ui_main->setupUi(this);
}
//-------------------------------------------------------------------------------------------------
main_window::~main_window() {
    delete ui_main;
}
//-------------------------------------------------------------------------------------------------
void main_window::display() {
    ui_auth.show();
}
//-------------------------------------------------------------------------------------------------
bool main_window::connectDB() {
//    mw_db = QSqlDatabase::addDatabase("QSQLITE");
//    mw_db.setDatabaseName("authorisation");

    mw_db = QSqlDatabase::addDatabase("QMYSQL");
    mw_db.setHostName("localhost");
    mw_db.setPort(3306);
    mw_db.setUserName("root");
    mw_db.setPassword("qwertyuiop00004326");
    mw_db.setDatabaseName("DB_user_DOG");

//    if(!mw_db.open()) {
//        qDebug() << "Cannot open database: " << mw_db.lastError();
//        return false;
//    }
    if (!mw_db.open()){
        qDebug() << "Database error connect" << mw_db.lastError().text();
        return false;
    }
    return true;
}
//-------------------------------------------------------------------------------------------------
void main_window::on_authorize_user() {
    if(!connectDB()) {
        return; // тут можно вывести окно "что-то пошло не так, попробуйте снова"
    }
    else {
        m_user_name = ui_auth.get_name();
        m_user_password = ui_auth.get_password();

        QString str_t = " SELECT * FROM user WHERE user_name = '%1'";

        QString user_name = "";
        QString user_password = "";

        db_input = str_t.arg(m_user_name);

        QSqlQuery query;    // запрос
        QSqlRecord rec;     // запись

        if(!query.exec(db_input)) {
            qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        }

        rec = query.record();       // получаем данные из запроса и записываем их в rec(запись)
        query.next();
//        m_user_counter = query.value(rec.indexOf("id")).toInt();
        user_name = query.value(rec.indexOf("user_name")).toString();
        user_password = query.value(rec.indexOf("user_password")).toString();

        if(m_user_name != user_name || m_user_password != user_password) {
            qDebug() << "Password missmatch" << user_name << " " << user_password;
            m_loginSuccesfull = false;
        }
        else {
            m_loginSuccesfull = true;

            ui_auth.close();
            ui_reg.close();
            this->show();
        }
    }
}
//-------------------------------------------------------------------------------------------------
void main_window::on_register_user() {
//    if(!connectDB()) {
//        return;
//    }
//    else {
        QSqlQuery query;    // запрос
//        QSqlRecord rec;     // запись
//        QString str_t = "SELECT id_user(*) FROM user;";
//        db_input = str_t;
//        if(!query.exec(db_input)) {
//            qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
//            return;
//        }
//        else {
//            query.next();
//            rec = query.record();
//            m_user_counter = rec.value(0).toInt();
//            qDebug() << m_user_counter;
//        }

        m_user_name = ui_reg.get_name();
        m_user_email = ui_reg.get_email();
        m_user_password = ui_reg.get_password();
//        m_user_counter++;

//INSERT INTO user (user_name, user_email, user_password) VALUES ("sullyshka", "sulla@gmail.com", "1234");
//        str_t = "INSERT INTO userlist(id, name, email, password)"
//                "VALUES('%1', '%2', '%3', '%4');";

        QString str_t = "INSERT INTO user (user_name, user_email, user_password)"
                        "VALUES('%1', '%2', '%3');";
        db_input = str_t.arg(m_user_name).arg(m_user_email).arg(m_user_password);

        if(!query.exec(db_input)) {
            qDebug() << "Unable to insert data"  << query.lastError() << " : " << query.lastQuery();
        }
        else {
            ui_reg.hide();
            ui_auth.show();
        }
//    }
}
//-------------------------------------------------------------------------------------------------
void main_window::on_authorize_window_show() {
    ui_reg.hide();
    ui_auth.show();
}
//-------------------------------------------------------------------------------------------------
void main_window::on_register_window_show() {
    ui_auth.hide();
    ui_reg.show();
}
//-------------------------------------------------------------------------------------------------
