#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

// simple Tcp server
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initServer();
    void closeServer();
    void updateState();

private:
    Ui::Widget *ui;

    QTcpServer *server;
    QList<QTcpSocket*> clientList;
};

#endif // WIDGET_H
