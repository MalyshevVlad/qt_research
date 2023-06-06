#ifndef USER_H
#define USER_H

#include <QTcpSocket>
#include <QHostAddress>

class User {
private:
    QString m_user_name;
    QTcpSocket* m_socket;

public:
    User();
    ~User();

    QTcpSocket* get_socket();
    QString get_user_name();

public slots:
    void sl_set_user_name(QString _name);
    void sl_set_socket(QTcpSocket* _socket);
};

#endif // USER_H
