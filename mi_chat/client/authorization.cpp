#include "authorization.h"
#include "ui_authorization.h"


/******************************************************************/
/*                                                                */
/*          сделай провеку на подключённость к хосту              */
/*                                                                */
/******************************************************************/



//-------------------------------------------------------------------------------------------------
Authorization::Authorization(QWidget *parent) : QWidget(parent), ui(new Ui::Authorization) {
    ui->setupUi(this);

    m_socket = new QTcpSocket;
    m_registration = new Registration(parent);
    m_password_hash = new Hash;


    socket->connectToHost("127.0.0.1", 17205);


    connect(ui->log_in, SIGNAL(clicked()), this, SLOT(sl_button_send_user()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(sl_get_message()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(close()));
    connect(m_registration, SIGNAL(sendData(QString)), this, SLOT(sl_recieve_data(QString)));

    ui->error_label->hide();
    ui->errorconnect_label->hide();
    ui->pass_enter->setEchoMode(QLineEdit::Password);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    this->show();
}
//-------------------------------------------------------------------------------------------------
Authorization::~Authorization() {
    delete ui;
    delete m_socket;
    delete m_registration;
    delete m_password_hash;
}
//-------------------------------------------------------------------------------------------------
void Authorization::mouse_move_event(QMouseEvent* _event) {
    if (_event->buttons() && Qt::LeftButton) {
        move(_event->globalPos() - m_dragPosition);
        _event->accept();
      }
}
