#ifndef SELECTCONTACTS_H
#define SELECTCONTACTS_H

#include <QWidget>

namespace Ui {
class SelectContacts;
}

class SelectContacts : public QWidget
{
    Q_OBJECT

public:
    explicit SelectContacts(QWidget *parent = nullptr);
    ~SelectContacts();

private:
    Ui::SelectContacts *ui;
};

#endif // SELECTCONTACTS_H
