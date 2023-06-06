#ifndef SQLDB_H
#define SQLDB_H

#include <QObject>
#include <QtSql>
#include <QPair>

/*
============================================
*                                          *
*     [+] добавь там где нужно const имя&  *
*     [+] Поменяй int на qint              *
*                                          *
============================================
*/

typedef QList <QPair <QString, QList<QPair <QString, QString> > > > ChatListVector;
typedef QList <QPair <QString, QString>> PairStringList;

class SQLdb : public QObject {
    Q_OBJECT

private:
    QSqlDatabase bet_we_Kalambet;
    QString check_password(const QString& _password);

public:
    explicit SQLdb(QObject *parent = nullptr);
    ~SQLdb();

public slots:
    void        sl_add_contact(const QString& _user_name, const QString& _sex, int _age, const QString& _city, const QString& _password, const QString& _public_key, const QString& _salt);
    void        sl_add_message_in_chat(const QString& _who, const QString& _find, const QString& _message, const QString& _login);
    void        sl_add_chat_table(const QString& _who, const QString& _find);
    void        sl_friend_list_name(const QString& _user_name, QList<QString>& _user_list);
    void        sl_create_group(QString _group_name, const QString& _group_description, const QList<QString>& _user_list);
    void        sl_clear_history(const QString& _from, const QString& _to);
    void        sl_delete_friend(const QString& _from, const QString& _delete_friend);
    void        sl_update_online_status(const QString& _status, const QString& _user_name);
    void        sl_update_all_data_of_user(const QList<QString>& _dataset);
    void        sl_update_state_notification_from_user(const QString& _from, const QString& _user_name, const QString& _state);
    QString     sl_friend_list(const QString& _user_name, quint32 _size_list);
    QString     sl_get_chat_history(const QString& _user_name);
    QString     sl_get_chat_history_per_user(const QString& _my_name, const QString& _user_name);
    QString     sl_get_full_user_informations(const QString& _user_name);
    QString     sl_get_only_public_key(const QString& _user_name);
    QString     sl_find_in_Kalambet(const QString& _user_name, const QString& _who_find);
    QString     sl_correct_input(const QString& _login, const QString& _password);
    QList<QString> sl_get_online_status(const QString& _user_name);
    QList<QString> sl_user_data(const QString& _user_name);
};

#endif // SQLDB_H
