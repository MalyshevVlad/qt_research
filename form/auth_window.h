#ifndef AUTH_WINDOW_H
#define AUTH_WINDOW_H

#include <QWidget>

namespace Ui { class auth_window; }


class auth_window : public QWidget {
    Q_OBJECT

private:
    Ui::auth_window *ui;
    QString m_user_name;
    QString m_user_password;

public:
    explicit auth_window(QWidget *parent = nullptr);
    ~auth_window();
    QString get_name();
    QString get_password();

signals:
    void sing_up_pushButton_clicked();
    void sing_in_pushButton_clicked();

private slots:
    void on_lineEdit_name_textEdited(const QString &arg);
    void on_lineEdit_password_textEdited(const QString &arg);
    void on_pushButton_sing_up_clicked();
    void on_pushButton_sing_in_clicked();
};

#endif // AUTH_WINDOW_H
