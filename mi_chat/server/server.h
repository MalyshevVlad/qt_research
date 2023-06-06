#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QFile>
#include <QPair>

#include "sqldb.h"
#include "user.h"


typedef QList <QPair <QString, QString>> PairStringList;


QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

private:
    Ui::Server *ui;
    SQLdb* m_data_base;
    QTcpServer* m_server;
    QList<User*> m_client_connections;              // список подключений

    QString time_connect();                         // время соединения
    quint32 m_next_block_size;
    QHash<QTcpSocket*, QByteArray*> m_buffers;
    QHash<QTcpSocket*, qint32*> m_sizes;
    QTimer* m_timer;

private slots:
    void sl_get_message();
    void sl_new_connect();
    void sl_disconnect();
    void sl_status();

    void sl_send_response_to_ID(const QString& _message, qint32 _id);     // отправка личных сообщений
    void sl_user_is_online(QTcpSocket* _client, const QString& _user);
    void sl_private_message(QTcpSocket* _client, const QList<QString>& _message_list);
    void sl_sending_file(QTcpSocket* _client);
    void sl_login(QTcpSocket* _client, const QString & _user_name, QString& _city, const QString& _password, const QString& _age, const QString& _sex, const QString& _public_key, const QString& _salt);
    void sl_notification_network(const QString& _user_name, const QList<QString>& _friend_list, qint32 _state);
    QList<QString> sl_request_separation(QString _text);

    void sl_pushButton_clicked();

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

};
#endif // SERVER_H
