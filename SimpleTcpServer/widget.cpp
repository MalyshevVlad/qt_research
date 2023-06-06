#include "widget.h"
#include "ui_widget.h"

#include <QHostAddress>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    initServer();
}

Widget::~Widget()
{
    closeServer();
    delete ui;
}

void Widget::initServer()
{
    server = new QTcpServer(this);

    // начать/завершить прослушивание сервера (listen)
    connect(ui->btnListen,&QPushButton::clicked,[this] {
        if (server->isListening()) {
            closeServer();

            ui->btnListen->setText("Listen");
            ui->editAddress->setEnabled(true);
            ui->editPort->setEnabled(true);
        }
        else {
            const QString address_text = ui->editAddress->text();
            const QHostAddress address = (address_text=="Any") ? QHostAddress::Any : QHostAddress(address_text);
            const unsigned short port = ui->editPort->text().toUShort();

            if(server->listen(address,port)) {
                ui->btnListen->setText("Close");
                ui->editAddress->setEnabled(false);
                ui->editPort->setEnabled(false);
            }
        }
        updateState();
    });

    // обработка нового подключения (incomingConnection)
    connect(server,&QTcpServer::newConnection,this,[this]{
        while(server->hasPendingConnections()) {
            QTcpSocket *socket=server->nextPendingConnection();
            clientList.append(socket);
            ui->textRecv->append(QString("[%1:%2] Soket Connected")
                                 .arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort()));

            connect(socket,&QTcpSocket::readyRead,[this,socket]{
                if(socket->bytesAvailable()<=0)
                    return;

                const QString receiver_text = QString::fromUtf8(socket->readAll());
                ui->textRecv->append(QString("[%1:%2]")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort()));
                ui->textRecv->append(receiver_text);
            });


            connect(socket,&QAbstractSocket::errorOccurred,[this,socket](QAbstractSocket::SocketError){
                ui->textRecv->append(QString("[%1:%2] Soket Error: %3")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort())
                                     .arg(socket->errorString()));
            });


            connect(socket,&QTcpSocket::disconnected,[this,socket]{
                socket->deleteLater();
                clientList.removeOne(socket);
                ui->textRecv->append(QString("[%1:%2] Soket Disonnected")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort()));
                updateState();
            });
        }
        updateState();
    });


    connect(ui->btnSend,&QPushButton::clicked,[this]{
        if(!server->isListening())
            return;

        const QByteArray send_data=ui->textSend->toPlainText().toUtf8();

        if(send_data.isEmpty())
            return;
        for(QTcpSocket *socket:clientList)
        {
            socket->write(send_data);
            //socket->waitForBytesWritten();
        }
    });


    connect(server,&QTcpServer::acceptError,[this](QAbstractSocket::SocketError){
        ui->textRecv->append("Server Error: "+server->errorString());
    });
}

void Widget::closeServer()
{

    server->close();
    for(QTcpSocket* socket: clientList) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState){
            socket->abort();
        }
    }
}

void Widget::updateState()
{
    if(server->isListening()) {
        setWindowTitle(QString("Server[%1:%2] connections: %3")
                       .arg(server->serverAddress().toString())
                       .arg(server->serverPort())
                       .arg(clientList.count()));
    }
    else {
        setWindowTitle("Server");
    }
}
