#ifndef OUTPUT_FORM_H
#define OUTPUT_FORM_H

#include <QWidget>

namespace Ui {
class output_form;
}

class output_form : public QWidget
{
    Q_OBJECT

public:
    explicit output_form(QWidget *parent = nullptr);
    ~output_form();

private:
    Ui::output_form *ui;
};

#endif // OUTPUT_FORM_H
