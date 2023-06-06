#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QTime>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

private:
    QPoint m_draw_position;
    Hash* m_password_hash;
    qint32 m_public_key;

protected:
    QTcpSocket* m_socket;
    void mouse_move_event(QMouseEvent* _event);
    void mouse_press_event(QMouseEvent* _event);
public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

    virtual void key_press_event(QKeyEvent* _event);
    virtual void key_release_event(QKeyEvent* _event);

    bool m_ok;
    Ui::Authorization *ui;
    Registration* m_registration;

signals:
    void si_send_data(QString _user_name, QString _password, QString _public_key);
    void si_send_find_contact(QString _user_name);

private slots:
    void sl_button_send_user();
    void sl_recieve_data(QString _recieve_date);
    void sl_get_message();

    void sl_turn_window_clicked();
    void sl_close_window_clicked();
    void sl_log_in_clicked();
    void sl_reg_button_clicked();

public slots:
    void sl_set_language(QMap<QString, QString>& _landuage);
};

#endif // AUTHORIZATION_H
