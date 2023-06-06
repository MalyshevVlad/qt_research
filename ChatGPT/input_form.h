#ifndef INPUT_FORM_H
#define INPUT_FORM_H

#include <QWidget>

namespace Ui {
class input_form;
}

class input_form : public QWidget
{
    Q_OBJECT

public:
    explicit input_form(QWidget *parent = nullptr);
    ~input_form();

private:
    Ui::input_form *ui;
};

#endif // INPUT_FORM_H
