#include "sqldb.h"


/*
=================================================================================================
*                                                                                               *
*     [-] переделай таблицу из  sl_create_group, а то реально кончено _group_description стоит  *
*     [+] вопрос об const& имя в сингатурах все ещё актуален                                    *
*     [-] когда на винде будешь, под mysql переделай, а то у тебя таблицы конкретно плывут      *
*     [-] блин Влада, ну кринж полнейший, давай по новой                                        *
*                                                                                               *
=================================================================================================
*/



/*-----------------------------------------------------------------------------------------------*/
SQLdb::SQLdb(QObject *parent) : QObject(parent)
{
    bet_we_Kalambet = QSqlDatabase::addDatabase("QSQLITE");

    QString path_to_DB("/home/banana/vlada/mi_chat/mi_chat/database/mydatabase.sqlite");
    bet_we_Kalambet.setDatabaseName(path_to_DB);

    QFileInfo check_file(path_to_DB);
    if (check_file.isFile()) {
        if (bet_we_Kalambet.open())
            qDebug() << "[+] Connected to Database File";
        else
            qDebug() <<"[!] Database File does not exist";
    }

    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare("CREATE TABLE users ("
                    "id_user       INT auto_increment PRIMARY KEY,"
                    "user_name     TEXT NOT NULL, "
                    "first_name    TEXT NOT NULL, "
                    "last_name     TEXT NOT NULL, "
                    "sex           TEXT NOT NULL, "
                    "age           INT  NOT NULL, "
                    "city          TEXT NOT NULL, "
                    "email_phone   TEXT NOT NULL, "
                    "user_password TEXT NOT NULL, "
                    "salt          TEXT NOT NULL, "
                    "online_satus  TEXT NOT NULL, "
                    "lives_status  TEXT NOT NULL, "
                    "public_key    TEXT NOT NULL  "
                    ")");

    if (!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    // тут таблицы которые создаются походу программы, пусть сдесь будут их sql-запросы, тк искать среди функций тяжело

    /* "CREATE TABLE friend_user_name (
            user_name text NOT NULL,
            sex text NOT NULL,
            public_key text NOT NULL,
            notifications text NOT NULL
        )"
    */

    /* "CREATE TABLE chat_who_find (message text NOT NULL, who text NOT NULL, time text NOT NULL)" */
    /* "CREATE TABLE group_name (user_name text NOT NULL)" */

}
/*-----------------------------------------------------------------------------------------------*/
SQLdb::~SQLdb()
{
    qDebug() << "Closing the connection to Database file on exist";
    bet_we_Kalambet.close();
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::check_password(const QString& _password)
{
    QCryptographicHash a_calculate_sha256(QCryptographicHash::Sha256);
    QByteArray in;
    in.append(_password.toUtf8());

    a_calculate_sha256.addData(in);
    QByteArray a_result = a_calculate_sha256.result().toHex();
    QString a_check_password = QString::fromUtf8(a_result.constData());
    return a_check_password;
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_add_contact(const QString& _user_name, const QString& _sex, qint32 _age, const QString& _city, const QString& _password, const QString& _public_key, const QString& _salt)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare(QString("INSERT INTO users ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')")
                           .arg(_user_name)
                           .arg(_sex)
                           .arg(_age)
                           .arg(_city)
                           .arg(_password)
                           .arg(_public_key)
                           .arg(_salt)
                           .arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"))
                  );

    if(!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }


    a_query.prepare(QString("CREATE TABLE friend_" + _user_name + " (user_name text NOT NULL, sex text NOT NULL, public_key text NOT NULL, notifications text NOT NULL)"));
    if (!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_add_message_in_chat(const QString& _who, const QString& _find, const QString& _message, const QString& _login)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare(QString("INSERT INTO chat_" + _who + "_" + _find + " (message, who, time) VALUES('%1', '%2', '%3')")
                            .arg(_message)
                            .arg(_login)
                            .arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm"))
                    );

    if (!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_add_chat_table(const QString& _who, const QString& _find)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare(QString("CREATE TABLE chat_" + _who + "_" + _find + " (message text NOT NULL, who text NOT NULL, time text NOT NULL)"));

    if (!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_friend_list_name(const QString& _user_name, QList<QString>& _user_list)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare(QString("SELECT user_name FROM friend_" + _user_name));

    if (a_query.exec()) {
        while (a_query.next())
            _user_list.push_back(a_query.value(0).toString());
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
// очень больно конечно так вставлять _group_description((((
void SQLdb::sl_create_group(QString _group_name, const QString& _group_description, const QList<QString>& _user_list)
{
    QSqlQuery a_query(bet_we_Kalambet);
    _group_name = _group_name.simplified().replace(" ", "_");
    QList<QString> a_strlist_query = {
        "CREATE TABLE group_" + _group_name + " (user_name text NOT NULL)",
        QString("INSERT INTO group_" + _group_name +  " (user_name) VALUES ('%1')").arg(_group_description),
        QString("INSERT INTO group_" + _group_name +  " (user_name) VALUES ('%1')").arg("===============")
    };


    for (qint32 i = 0; i < a_strlist_query.size() - 1; i++) {
        if (!a_query.exec(a_strlist_query[i])) {
            qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
        }
    }


    for(qint32 i = 0; i < _user_list.size(); i++) {
        a_query.prepare(QString("INSERT INTO group_" + _group_name +  " (user_name) VALUES ('%1')").arg(_user_list.at(i)));
        if (!a_query.exec()) {
            qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
        }
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_clear_history(const QString& _from, const QString& _to)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare("DELETE FROM chat_" + _from + _to);

    if(!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
// Удалении происходит только на "исходном" клиенте. Удаляемый пользователь потом может написать тому, кто удалил, и у того
// вновь добавится человек, которого удалил. СложнА :))
void SQLdb::sl_delete_friend(const QString& _from, const QString& _delete_friend)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare("DELETE FROM friend_" + _from + "WHERE user_name = '" + _delete_friend + "'");

    if(!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_update_online_status(const QString& _status, const QString& _user_name)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QList<QString> a_strlist_query = {
        "UPDATE users SET online_status = 'online' WHERE user_name = '" + _user_name + "'",
        QString("UPDATE users SET online_status = '%1' WHERE user_name = '" + _user_name + "'").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"))
    };

    if (_status == "online") {
        if(!a_query.exec(a_strlist_query[0])) {
            qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
        }
    }
    else if (_status == "offline") {
        if(!a_query.exec(a_strlist_query[1])) {
            qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
        }
    }
    else {
        qDebug() << "[!] uncorrected status";
    }
}
/*-----------------------------------------------------------------------------------------------*/
void SQLdb::sl_update_all_data_of_user(const QList<QString>& _dataset)
{
    QSqlQuery a_query(bet_we_Kalambet);
//    qDebug() << _dataset;
    a_query.prepare(QString("UPDATE users SET first_name = '%1', last_name = '%2', email_phone = '%3', sex = '%4', age = '%5', city = '%6', lives_status = '%7' WHERE user_name = '%8'")
            .arg(_dataset[1])
            .arg(_dataset[2])
            .arg(_dataset[3])
            .arg(_dataset[4])
            .arg(_dataset[5])
            .arg(_dataset[6])
            .arg(_dataset[7])
            .arg(_dataset[0]));

    if(!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
// возможно тут херня, было: "UPDATE friend = '%1' SET notifications = '%2' WHERE user_name = '" + _user_name + "'").arg(_from).arg(_state));
void SQLdb::sl_update_state_notification_from_user(const QString& _from, const QString& _user_name, const QString& _state)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare(QString("UPDATE friend_'%1' SET notifications = '%2' WHERE user_name = '" + _user_name + "'").arg(_from).arg(_state));

    if(!a_query.exec()) {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_friend_list(const QString& _user_name, quint32 _size_list)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QString a_friend_info;
    int i = 0;
    QList<QString> a_more_data = sl_get_online_status(_user_name);

    a_query.prepare("SELECT user_name, sex, public_key, notifications FROM friend_" + _user_name);
    if(a_query.exec()) {
        while (a_query.next()) {
            a_friend_info.append(a_query.value(0).toString() + "_"
                                 + a_query.value(1).toString() + "_"
                                 + a_query.value(2).toString() + "_"
                                 + a_query.value(3).toString()
                                 + a_more_data[i++] + "/s"
                                 );
        }
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    _size_list = a_more_data.size();
    return a_friend_info;
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_get_chat_history(const QString& _user_name)
{
    QSqlQuery a_query_user(bet_we_Kalambet);
    QSqlQuery a_query_chat(bet_we_Kalambet);
    QString a_chat_history = "";

    a_query_user.prepare("SELECT user_name FROM friend_" + _user_name);

    if (a_query_user.exec()) {
        while (a_query_user.next()) {
            a_query_chat.prepare("SELECT message, who, time FROM chat_" + _user_name + a_query_user.value(0).toString());
            if (a_query_chat.exec()) {
                if (a_query_chat.next()) {
                    a_chat_history.append(" //s " + a_query_user.value(0).toString());
                    a_chat_history.append(" /pm " + a_query_chat.value(1).toString() + " /s " + a_query_chat.value(0).toString() + " /s " + a_query_chat.value(2).toString());
                }
                while (a_query_chat.next()) {
                    a_chat_history.append(" /pm " + a_query_chat.value(1).toString() + " /s " + a_query_chat.value(0).toString() + " /s " + a_query_chat.value(2).toString());
                }
            }
            else {
                qDebug() << "[!] " << a_query_chat.lastError() << " : " << a_query_chat.lastQuery();
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query_user.lastError() << " : " << a_query_user.lastQuery();
    }
    return a_chat_history;
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_get_chat_history_per_user(const QString& _my_name, const QString& _user_name)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QString a_chat_history = "";

    a_query.prepare("SELECT who, message, time FROM chat_" + _my_name + "_" + _user_name);

    if (a_query.exec()) {
        a_chat_history.append("/pm " + a_query.value(0).toString() + "/s " + a_query.value(1).toString() + "/s " + a_query.value(2).toString());
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    return a_chat_history;
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_get_full_user_informations(const QString& _user_name)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QString a_info_user = "INFP";

    a_query.prepare("SELECT user_name, sex, age, city, online_status, email_phone, lives_status FROM users WHERE user_name = '" + _user_name + "'");
    if (a_query.exec()) {
        if (a_query.next()) {
            if (a_query.value(0).toString() == _user_name) {
                a_info_user.append(a_query.value(0).toString() + "/s "
                                   + a_query.value(1).toString() + "/s "
                                   + a_query.value(2).toString() + "/s "
                                   + a_query.value(3).toString() + "/s "
                                   + a_query.value(4).toString() + "/s "
                                   + a_query.value(5).toString() + "/s "
                                   + a_query.value(6).toString() + "/s "
                                   + a_query.value(7).toString() + "/s ");
                return a_info_user;
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    return "INFN";
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_get_only_public_key(const QString& _user_name)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QString a_info_user = "KEYP";

    a_query.prepare("SELECT user_name, public_key FROM users WHERE user_name = '" + _user_name + "'");
    if (a_query.exec()) {
        if (a_query.next()) {
            if (a_query.value(0).toString() == _user_name) {
                a_info_user.append(a_query.value(1).toString());
                return a_info_user;
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }
    a_query.exec();
    return "KEYN";

}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_find_in_Kalambet(const QString& _user_name, const QString& _who_find) {
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare("SELECT user_name, sex, age, public_key, online_status, email_phone, lives_status FROM users WHERE user_name = '" + _user_name + "'");

    if (a_query.exec()) {
        if (a_query.next()) {
            if (a_query.value(0).toString() == _user_name) {
                if (!_who_find.isEmpty()) {
                    QSqlQuery query(bet_we_Kalambet);
                    query.prepare(QString("INSERT INTO friend_" + _who_find + " (user_name, sex, public_key, notifications) VALUES ('%1', '%2', '%3, '%4)")
                                  .arg(_user_name)
                                  .arg(a_query.value(1).toString())
                                  .arg(a_query.value(2).toString())
                                  .arg("YES")
                                  );
                    query.exec();
                }
                return  a_query.value(1).toString() + "/s " +
                        a_query.value(2).toString() + "/s " +
                        a_query.value(3).toString() + "/s " +
                        a_query.value(4).toString() + "/s " +
                        a_query.value(5).toString();
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    a_query.exec();
    return "false";
}
/*-----------------------------------------------------------------------------------------------*/
QString SQLdb::sl_correct_input(const QString& _login, const QString& _password)
{
    QSqlQuery a_query(bet_we_Kalambet);
    a_query.prepare("SELECT user_name, user_password, public_key, salt FROM users WHERE user_name = '" + _login + "'");

    if (a_query.exec()) {
        if (a_query.next()) {
            if (a_query.value(0).toString() == _login) {
                QString a_salt(a_query.value(3).toString());
                QString a_check_password(check_password(_password + a_salt));

                if (a_check_password == a_query.value(1).toString())
                    return a_query.value(2).toString();
               else return "false";
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query.lastError() << " : " << a_query.lastQuery();
    }

    a_query.exec();
    return "false";
}
/*-----------------------------------------------------------------------------------------------*/
// ну и херь конечно, зачем теме QStringList, если ты это как одну строку пихаешь?
QList<QString> SQLdb::sl_get_online_status(const QString& _user_name)
{
    QSqlQuery a_query_search(bet_we_Kalambet);
    QSqlQuery a_query_add(bet_we_Kalambet);
    QList<QString> a_user_data;

    a_query_search.prepare("SELECT user_name FROM friend_" + _user_name);
    if (a_query_search.exec()) {
        while (a_query_search.next()) {
            a_query_add.prepare("SELECT user_name, online_status, email_phone, lives_status FROM users WHERE user_name = '" + a_query_search.value(0).toString() + "'");
            if (a_query_add.exec()) {
                a_query_add.next();
                a_user_data.push_back(a_query_add.value(0).toString() + " _ " + a_query_add.value(1).toString() + " _ " + a_query_add.value(2).toString() + " _ " + a_query_add.value(3).toString());
            }
            else {
                qDebug() << "[!] " << a_query_add.lastError() << " : " << a_query_add.lastQuery();
            }
        }
    }
    else {
        qDebug() << "[!] " << a_query_search.lastError() << " : " << a_query_search.lastQuery();
    }

    return a_user_data;
}
/*-----------------------------------------------------------------------------------------------*/
QList<QString> SQLdb::sl_user_data(const QString& _user_name)
{
    QSqlQuery a_query(bet_we_Kalambet);
    QList<QString> a_user_list;

    if(a_query.exec("SELECT user_name, sex, age, city, live_status, email_phone FROM users WHERE user_name = '" + _user_name + "'")) {
        if(a_query.next())
            if (a_query.value(0).toString() == _user_name)
            {
                a_user_list.push_back(_user_name);
                a_user_list.push_back(a_query.value(1).toString());
                a_user_list.push_back(a_query.value(2).toString());
                a_user_list.push_back(a_query.value(3).toString());
                a_user_list.push_back(a_query.value(4).toString());
                a_user_list.push_back(a_query.value(5).toString());
                return a_user_list;
            }
    }
    a_query.exec();
    return a_user_list;
}
/*-----------------------------------------------------------------------------------------------*/
