#ifndef HOME_FORM_H
#define HOME_FORM_H

#include <QWidget>

namespace Ui {
class home_form;
}

class home_form : public QWidget
{
    Q_OBJECT

public:
    explicit home_form(QWidget *parent = nullptr);
    ~home_form();

private:
    Ui::home_form *ui;
};

#endif // HOME_FORM_H
