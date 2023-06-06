#include "widget.h"
#include "ui_widget.h"

#include <QHostAddress>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Client");

    initClient();
}

Widget::~Widget()
{
    client->abort();
    delete ui;
}

void Widget::initClient()
{
    client = new QTcpSocket(this);

    // создание клиенского сокета
    connect(ui->btnConnect,&QPushButton::clicked,[this]{
        if(client->state()==QAbstractSocket::ConnectedState){
            client->abort();
        }
        else if (client->state()==QAbstractSocket::UnconnectedState) {
            const QHostAddress address=QHostAddress(ui->editAddress->text());
            const unsigned short port=ui->editPort->text().toUShort();

            client->connectToHost(address,port);
        }
        else {
            ui->textRecv->append("It is not ConnectedState or UnconnectedState");
        }
    });


    connect(client,&QTcpSocket::connected,[this]{
        ui->btnConnect->setText("Disconnect");
        ui->editAddress->setEnabled(false);
        ui->editPort->setEnabled(false);
        updateState();
    });


    connect(client,&QTcpSocket::disconnected,[this]{
        ui->btnConnect->setText("Connect");
        ui->editAddress->setEnabled(true);
        ui->editPort->setEnabled(true);
        updateState();
    });

    // запись данных в сокет (write)
    connect(ui->btnSend,&QPushButton::clicked,[this]{
        if(!client->isValid())
            return;

        const QByteArray send_data = ui->textSend->toPlainText().toUtf8();
        if(send_data.isEmpty())
            return;
        client->write(send_data);
        //client->waitForBytesWritten();
    });

    // чтение данных (readyRead)
    connect(client,&QTcpSocket::readyRead,[this]{
        if(client->bytesAvailable()<=0)
            return;

        const QString recv_text=QString::fromUtf8(client->readAll());
        ui->textRecv->append(QString("[%1:%2]")
                             .arg(client->peerAddress().toString())
                             .arg(client->peerPort()));
        ui->textRecv->append(recv_text);
    });


    connect(client,&QAbstractSocket::errorOccurred,[this](QAbstractSocket::SocketError){
        ui->textRecv->append("Socket Error:"+client->errorString());
    });
}

void Widget::updateState()
{
    if(client->state()==QAbstractSocket::ConnectedState){
        setWindowTitle(QString("Client[%1:%2]")
                       .arg(client->localAddress().toString())
                       .arg(client->localPort()));
    }
    else {
        setWindowTitle("Client");
    }
}

