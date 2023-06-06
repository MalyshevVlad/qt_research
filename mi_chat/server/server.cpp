#include "server.h"
#include "ui_server.h"

/*
=================================================================================================
*                                                                                               *
*     [-] добавить проверку на корректность данных при регистрации (case 5)                     *
*                                                                                               *
=================================================================================================
*/

template <typename T>
QSet<T> QListToQSet(const QList<T>& q_list) {
    return QSet<T> (q_list.constBegin(), q_list.constEnd());
}

/*-----------------------------------------------------------------------------------------------*/
Server::Server(QWidget *parent) : QMainWindow(parent), ui(new Ui::Server) {
    ui->setupUi(this);

    m_data_base = new SQLdb;
    m_server = new QTcpServer;
    m_timer = new QTimer;
    m_next_block_size = 0;

    if (!m_server->listen(QHostAddress::Any, 17205)) {
        QMessageBox::critical(this, tr("chat server"), tr("Unable to start the server: %1.").arg(m_server->errorString()));
        close();       // закрываем сервер
        return;
    }

    connect(m_server, SIGNAL(newConnection()), this, SLOT(sl_new_connect()));
}
/*-----------------------------------------------------------------------------------------------*/
Server::~Server() {
    delete ui;
    delete m_data_base;
    delete m_server;
    delete m_timer;
}
/*-----------------------------------------------------------------------------------------------*/
QString Server::time_connect() {
    return "[" + QDateTime::currentDateTime().toString() + "]";
}
/*-----------------------------------------------------------------------------------------------*/
void Server::sl_get_message() {
//    QMap<qint16, QString> command;
//    qint16 a_current_command;
//    QStringList a_split_words;
//    QTcpSocket* a_client_socket = static_cast<QTcpSocket*>(QObject::sender());
//    QString a_type_packet = a_client_socket->read(4);


//    // просто заполняем map
//    QSet<QString> temp = {"LOAD", "MESG", "FIND", "FILE", "REGI", "AUTH", "CLNH", "DELF", "UINF", "NGRP", "GETI", "UPUI", "LGHT", "LCPU", "UPNU", "GETK"};
//    for (qint16 i = 0; i < temp.size(); i++) {
//        command[i + 1] = temp.at(i);
//    }


//    QMapIterator<QString, int> i(command);
//    while (i.hasNext()) {
//        i.next();
//        if (i.value() == a_type_packet)
//            a_current_command = i.key();
//    }


    // ну раз по конченому, так по конченому

    QList<QString> a_split_words;
    QTcpSocket* a_client_socket = static_cast<QTcpSocket*>(QObject::sender());
    QString a_type_packet = a_client_socket->read(4);

    qint16 command = 0;

    if (a_type_packet == "LOAD")
        command = 1;

    else if (a_type_packet == "MESG")
        command = 2;

    else if (a_type_packet == "FIND")
        command = 3;

    else if (a_type_packet == "FILE")
        command = 4;

    else if (a_type_packet == "REGI")
        command = 5;

    else if (a_type_packet == "AUTH")
        command = 6;

    else if (a_type_packet == "CLNH")
        command = 7;

    else if (a_type_packet == "DELF")
        command = 8;

    else if (a_type_packet == "UINF")
        command = 9;

    else if (a_type_packet == "NGRP")
        command = 10;

    else if (a_type_packet == "GETI")
        command = 11;

    else if (a_type_packet == "UPUI")
        command = 12;

    else if (a_type_packet == "LCHT")
        command = 13;

    else if (a_type_packet == "LCPU")
        command = 14;

    else if (a_type_packet == "UPNU")
        command = 15;

    else if (a_type_packet == "GETK")
        command = 16;

    switch (command) {
    case 1: {
        // LOAD
        QString a_new_user = a_client_socket->readAll();
        sl_user_is_online(a_client_socket, a_new_user);
        break;
    }

    case 2: {
        // MESG
        a_split_words = sl_request_separation(a_client_socket->readAll());
        if (a_split_words.size() == 4)
            sl_private_message(a_client_socket, a_split_words);
        break;
    }
    case 3: {
        // FIND
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 2) {
            QString a_find_user = a_split_words[0];
            QString a_who_find  = a_split_words[1];

            QString a_result = m_data_base->sl_find_in_Kalambet(a_find_user, a_who_find);

            if (a_result != "false") {
                // FNDP - Find Positive
                // FNDN - Find Negative
                sl_send_response_to_ID("FNDP" + a_result, a_client_socket->socketDescriptor());
                m_data_base->sl_add_chat_table(a_who_find, a_find_user);
                m_data_base->sl_add_chat_table(a_find_user, a_who_find);
                m_data_base->sl_find_in_Kalambet(a_who_find, a_find_user);
                ui->chat_dialog->addItem(time_connect() + " - " + a_who_find + " and " + a_find_user + " are now friends");

                for (auto client: m_client_connections) {
                    if (client->get_user_name() == a_find_user) {
                        a_result = m_data_base->sl_find_in_Kalambet(a_who_find, 0);
                        sl_send_response_to_ID("INVT" + a_who_find + "/s" + a_result, client->get_socket()->socketDescriptor());
                        break;
                    }
                }
            }
            else {
                sl_send_response_to_ID("FNDN", a_client_socket->socketDescriptor());
            }
        }
        break;
    }
    case 4: {
        // FILE
        sl_sending_file(a_client_socket);
        m_next_block_size = 0;
        break;
    }
    case 5: {
        // REGI
        // у тебя вообще нет проверки на корректность данных
        a_split_words = sl_request_separation(a_client_socket->readAll());
        if (a_split_words.size() == 7) {
            sl_login(a_client_socket, a_split_words[0], a_split_words[1], a_split_words[2],
                    a_split_words[3], a_split_words[4], a_split_words[5], a_split_words[6]);
        }
        break;
    }
    case 6: {
        // AUTH
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 2) {
            QString a_login = a_split_words[0];
            QString a_password = a_split_words[1];
            QString a_result = m_data_base->sl_correct_input(a_login, a_password);

            if (a_result == "false") {
                a_client_socket->write(QString("ERRA").toUtf8());
            }
            else {
                a_client_socket->write(QString("OKEY").toUtf8());
                a_client_socket->write(a_result.toUtf8());
            }
        }
        break;
    }
    case 7: {
        // CLNH
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 2) {
            QString a_from = a_split_words[0];
            QString a_to = a_split_words[1];

            m_data_base->sl_clear_history(a_from, a_to);
        }
        break;
    }
    case 8: {
        // DELF
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 2) {
            QString a_from = a_split_words[0];
            QString a_to = a_split_words[1];

            m_data_base->sl_delete_friend(a_from, a_to);
        }
        break;
    }
    case 9: {
        // UINF
//        QString a_user_info = a_client_socket->readAll();
//        QStringList a_data_list = m_data_base->userData(a_user_info);

        // FIXME
        // a_client_socket->write(QString("UINF" + a_data_list).toUtf8());
        a_client_socket->write(QString("UINF").toUtf8());
        break;
    }
    case 10: {
        // NGRP
        QList<QString> a_user_list;
        QString a_group_name, group_description;

        // FIXME
        // in >> a_group_name >> group_description >> a_user_list;
        m_data_base->sl_create_group(a_group_name, group_description, a_user_list); // ты пустые строки передаёшь в функцию, ну и фигня...
        break;
    }
    case 11: {
        // GETI
        QString a_user = a_client_socket->readAll();
        QString a_user_info = m_data_base->sl_get_full_user_informations(a_user);
        a_client_socket->write(a_user_info.toUtf8());
        break;
    }
    case 12: {
        // UPUI
        a_split_words = sl_request_separation(a_client_socket->readAll());
        m_data_base->sl_update_all_data_of_user(a_split_words);
        break;
    }
    case 13: {
        // LGHT
        QString a_user = a_client_socket->readAll();
        QString a_chat_history = m_data_base->sl_get_chat_history(a_user);

        if (a_chat_history.isEmpty()) {
            a_client_socket->write(QString("EMPT").toUtf8());
        }
        else {
            a_client_socket->write(QString("EXST").toUtf8());
            a_client_socket->write(a_chat_history.toUtf8());
        }
        break;
    }
    case 14: {
        // LCPU
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 2) {
            QString a_from = a_split_words[0];
            QString a_user_dialog = a_split_words[1];
            QString a_chat_history = m_data_base->sl_get_chat_history_per_user(a_from, a_user_dialog);

            if (a_chat_history.isEmpty()) {
                a_client_socket->write(QString("EMPT").toUtf8());
            }
            else {
                a_client_socket->write(QString("PERU").toUtf8());
                a_client_socket->write(a_chat_history.toUtf8());
            }
        }
        break;
    }
    case 15: {
        // UPNU
        a_split_words = sl_request_separation(a_client_socket->readAll());

        if (a_split_words.size() == 3) {
            QString a_from = a_split_words[0];
            QString a_user = a_split_words[1];
            QString a_state = a_split_words[2];

            m_data_base->sl_update_state_notification_from_user(a_from, a_user, a_state);
        }
        break;
    }
    case 16: {
        // GETK
        QString a_user = a_client_socket->readAll();
        QString a_user_public_key = m_data_base->sl_get_only_public_key(a_user);
        a_client_socket->write(a_user_public_key.toUtf8());
        break;
    }
    default:
        break;

    }
}
//-------------------------------------------------------------------------------------------------
void Server::sl_new_connect() {
    // создаём новый сокет
    User* a_new_user = new User;
    a_new_user->sl_set_socket(m_server->nextPendingConnection());
    QTcpSocket* a_new_socket = a_new_user->get_socket();

    // даём ему всякие там конекты
    connect(a_new_socket, SIGNAL(disconnected()), this, SLOT(sl_disconnect()));
    connect(a_new_socket, SIGNAL(readyRead()), this, SLOT(sl_get_message()));

    qDebug() << "[+] new connection: " << a_new_socket->socketDescriptor();

    // закидываем наш сокет и массив байт в хеш-массив
    QByteArray* a_buffer = new QByteArray;
    qint32* a_size = new qint32(0);
    m_buffers.insert(a_new_socket, a_buffer);
    m_sizes.insert(a_new_socket, a_size);
}
//-------------------------------------------------------------------------------------------------
void Server::sl_disconnect() {
    QTcpSocket* a_socket = qobject_cast<QTcpSocket*>(sender());
    QList<QString> a_user_list;

    qDebug() << "[-] disconnect user";

    User* a_disconnect_user;
    for (auto client: m_client_connections) {
        if (client->get_socket() == a_socket) {
            a_disconnect_user = client;

            if (!ui->user_list->size().isEmpty()) {
                for (qint32 j = 0; j < ui->user_list->count(); j++) {
                    if (ui->user_list->item(j)->data(Qt::DisplayRole) == a_disconnect_user->get_user_name()) {
                        ui->user_list->removeItemWidget(ui->user_list->takeItem(j));
                        ui->chat_dialog->addItem(time_connect() + " - " + a_disconnect_user->get_user_name() + " disconnected");
                        m_data_base->sl_update_online_status("offline", a_disconnect_user->get_user_name());
                        m_data_base->sl_friend_list_name(a_disconnect_user->get_user_name(), a_user_list);
                        sl_notification_network(a_disconnect_user->get_user_name(), a_user_list, 0);
                    }
                }
            }
        }
    }
    m_client_connections.removeAll(a_disconnect_user);
}
//-------------------------------------------------------------------------------------------------
void Server::sl_status() {
    for (auto client: m_client_connections) {
        qDebug() << "[*] " << client->get_user_name();
    }
}
//-------------------------------------------------------------------------------------------------
void Server::sl_send_response_to_ID(const QString& _message, qint32 _id) {
    // отправка конкретному клиенту
    for (auto client: m_client_connections) {
        if (client->get_socket()->socketDescriptor() == _id) {
            client->get_socket()->write(_message.toUtf8());
            break;
        }
    }
}
//-------------------------------------------------------------------------------------------------
void Server::sl_user_is_online(QTcpSocket* _client, const QString& _user) {
    QString a_response = "FLST";
    QString a_user_name = _user;
    QString a_tmpn_name = a_user_name;
    bool a_already_name = true;
    qint32 a_num_inc = 0;
    qint32 a_count_user = 0;
    QString a_friend_list;

    m_data_base->sl_update_online_status("online", a_user_name);
    a_friend_list = m_data_base->sl_friend_list(a_user_name, a_count_user);

    if (a_count_user > 0) {
        a_response.append(a_friend_list);
        _client->write(a_response.toUtf8());
    }
    else {
        _client->write(QString("NOFL").toUtf8());
    }

    while (a_already_name) {
        a_already_name = false;

        for (auto client: m_client_connections) {
            if (client->get_user_name() == a_user_name) {
                a_already_name = true;
                break;
            }
        }
        if (a_already_name) {
            a_tmpn_name = a_user_name + "(" + QString::number(a_num_inc) + ")";
            ++a_num_inc;
        }
    }

    a_user_name = a_tmpn_name;
    for (auto client: m_client_connections) {
        if (client->get_socket() == _client) {
            client->sl_set_user_name(a_user_name);
            ui->user_list->addItem(client->get_user_name());
            ui->chat_dialog->addItem(time_connect() + " - " + client->get_user_name() + " is online (" + QString::number(_client->socketDescriptor()) + ")");
            break;
        }
    }

}
//-------------------------------------------------------------------------------------------------
void Server::sl_private_message(QTcpSocket* _client, const QList<QString>& _message_list) {
    qDebug() << "[*] message list: " << _message_list;

    QString a_from_user = _message_list[0];
    QString a_from_msg  = _message_list[2];

    QString a_to_user   = _message_list[1];
    QString a_to_msg    = _message_list[3];

    User* a_to_user_socket = nullptr;

    if (!a_to_msg.isEmpty()) {
        for (auto client: m_client_connections) {
            if (client->get_user_name() == a_to_user) {
                a_to_user_socket = client;
                break;
            }
        }
    }
//    if (!a_to_msg.isEmpty()) {
//        qint16 a_index_client = m_client_connections.indexOf(a_to_user);
//        if (a_index_client != -1) {
//            a_to_user_socket = m_client_connections[a_index_client];
//        }
//        else {
//            qDebug() << "[!] unable to find client connection";
//        }
//    }

    ui->chat_dialog->addItem(time_connect() + " - PM: " + a_from_user + " -> " + a_to_user + ":  " + a_to_msg);

    if (a_to_user_socket != nullptr && !a_to_msg.isEmpty()) {
        QString a_new_message = "NMSG" + a_from_user + " /s " + a_to_msg;
        sl_send_response_to_ID(a_new_message, a_to_user_socket->get_socket()->socketDescriptor());
    }

    m_data_base->sl_add_message_in_chat(a_from_user, a_to_user, a_from_msg, QString("To"));
    m_data_base->sl_add_message_in_chat(a_to_user, a_from_user,  a_to_msg, QString("From"));

}
//-------------------------------------------------------------------------------------------------
void Server::sl_sending_file(QTcpSocket* _client) {
    QTcpSocket* a_client_socket = (QTcpSocket*)sender();
    QDataStream in(a_client_socket);
    in.setVersion(QDataStream::Qt_5_10);

    QByteArray a_buffer;
    QString    a_file_name;
    QString    a_receiver_name;
    QString    a_from_name;
    qint64     a_file_size;

    in >> a_receiver_name >> a_file_name >> a_file_size; // а зОчем? \(* ^ *)/ пустые то данные туда

    QString a_dir_downloads = QDir::homePath() + "/mi_files/" + a_receiver_name + "/";
    QDir(a_dir_downloads).mkdir(a_dir_downloads);

    QThread::sleep(1);


    forever {
        if (!m_next_block_size) {
            if (quint32(a_client_socket->bytesAvailable()) < sizeof(quint32))
                break;

            in >> a_buffer;
        }
        for (auto client: m_client_connections)
            if (client->get_socket() == _client)
                a_from_name = client->get_user_name();

        for (auto client: m_client_connections)
            if (client->get_user_name() == a_receiver_name) {
                QByteArray a_array_block;
                QDataStream out(&a_array_block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_10);

                out << quint32(0) << QString("GETF") << a_from_name << a_file_name << a_file_size << a_buffer;
                out.device()->seek(0);
                out << quint32(a_array_block.size() - sizeof(quint32));

                client->get_socket()->write(a_array_block);

            }
        if (a_client_socket->bytesAvailable() < m_next_block_size)
            break;
    }

    ui->chat_dialog->addItem(time_connect() + " - PM: " + a_from_name + " -> " + a_receiver_name + ": FILE " + a_file_name);
    m_data_base->sl_add_message_in_chat(a_from_name, a_receiver_name, "FILE " + a_file_name, QString("To ") + QString::number(qfloat16(a_file_size) / 1024, 'f', 2) + " KB");
    m_data_base->sl_add_message_in_chat(a_receiver_name, a_from_name, "FILE " + a_file_name, QString("From ") + QString::number(qfloat16(a_file_size) / 1024, 'f', 2) + " KB");

    QFile a_receive_file(a_dir_downloads + a_file_name);
    if (a_receive_file.open(QIODevice::ReadWrite)) {
        a_receive_file.write(a_buffer);
        a_receive_file.close();
        a_buffer.clear();
    }
    else {
        qDebug() << "[!] can't open file(" + a_dir_downloads + a_file_name + ")";
    }

    m_next_block_size = 0;
}
//-------------------------------------------------------------------------------------------------
void Server::sl_login(QTcpSocket* _client, const QString & _user_name, QString& _city, const QString& _password,
             const QString& _age, const QString& _sex, const QString& _public_key, const QString& _salt) {
        if (_user_name.isEmpty() || _password.isEmpty()) {
            _client->write(QString("PNIL").toUtf8());
        }
        else if (m_data_base->sl_find_in_Kalambet(_user_name, nullptr) == "false") {
            if (_city.isEmpty())
                _city = "unknown";

            m_data_base->sl_add_contact(_user_name, _sex, _age.toInt(), _city, _password, _public_key, _salt);
            ui->chat_dialog->addItem(time_connect() + " - user registration: " + _user_name);
            qDebug() << "[+] new user: " << _user_name;

            _client->write(QString("WELC").toUtf8());
        }
        else {
            _client->write(QString("ALRD").toUtf8());
        }
}
//-------------------------------------------------------------------------------------------------
void Server::sl_notification_network(const QString& _user_name, const QList<QString>& _friend_list, qint32 _state) {
    QList<QString> a_online_now;
    QString a_message = (_state == 0 ? "STOF" : "STON") + _user_name;

    for (qint32 i = 0; i < ui->user_list->count(); i++) {
        a_online_now.push_back(ui->user_list->item(i)->text());


        // удаляем все элементы из _friend_list, которые не содержатся в a_online_now
        QSet<QString> a_who_alert = QListToQSet(_friend_list).intersect(QListToQSet(a_online_now));

        for (auto client: m_client_connections) {
            for (qint32 j = 0; a_who_alert.size(); j++) {
                if (a_who_alert.contains(client->get_user_name())) {
                    sl_send_response_to_ID(a_message, client->get_socket()->socketDescriptor());
                    break;
                }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------
QList<QString> Server::sl_request_separation(QString _text) {
    QList<QString> a_list = _text.split("/s");
    return a_list;
}
//-------------------------------------------------------------------------------------------------
void Server::sl_pushButton_clicked() {
    qDebug() << m_client_connections.at(0)->get_socket()->write(QString("test " + ui->line_edit->text()).toUtf8()) << "\n"
             << "Test: " << ui->line_edit->text();
}
//-------------------------------------------------------------------------------------------------
void check_string(QString &response)
{
    if (response.size() > 4) {
        response = response.left(response.size() - 4);
        response.append("//s");
    }
}
//-------------------------------------------------------------------------------------------------
