#ifndef CHOICECREATE_H
#define CHOICECREATE_H

#include <QWidget>

namespace Ui {
class ChoiceCreate;
}

class ChoiceCreate : public QWidget
{
    Q_OBJECT

public:
    explicit ChoiceCreate(QWidget *parent = nullptr);
    ~ChoiceCreate();

private:
    Ui::ChoiceCreate *ui;
};

#endif // CHOICECREATE_H
