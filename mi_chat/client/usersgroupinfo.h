#ifndef USERSGROUPINFO_H
#define USERSGROUPINFO_H

#include <QWidget>

namespace Ui {
class UsersGroupInfo;
}

class UsersGroupInfo : public QWidget
{
    Q_OBJECT

public:
    explicit UsersGroupInfo(QWidget *parent = nullptr);
    ~UsersGroupInfo();

private:
    Ui::UsersGroupInfo *ui;
};

#endif // USERSGROUPINFO_H
