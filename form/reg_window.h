#ifndef REG_WINDOW_H
#define REG_WINDOW_H

#include <QWidget>

namespace Ui { class reg_window; }

class reg_window : public QWidget {
    Q_OBJECT

private:
    Ui::reg_window *ui;
    QString m_user_name;
    QString m_user_email;
    QString m_user_password;
public:
    explicit reg_window(QWidget *parent = nullptr);
    ~reg_window();
    QString get_name();
    QString get_email();
    QString get_password();

    bool check_name(const QString& arg);
    bool check_email(const QString& arg);
    bool check_password(const QString& arg);

signals:
    void sing_up_pushButton_clicked();
    void sing_in_pushButton_clicked();

private slots:
    void on_lineEdit_name_textEdited(const QString &arg);
    void on_lineEdit_email_textEdited(const QString &arg);
    void on_lineEdit_password_textEdited(const QString &arg);
    void on_pushButton_sing_up_clicked();
    void on_pushButton_sing_in_clicked();
};
#endif // REG_WINDOW_H
