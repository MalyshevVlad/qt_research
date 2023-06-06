#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QWidget>

namespace Ui {
class CreateGroup;
}

class CreateGroup : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroup(QWidget *parent = nullptr);
    ~CreateGroup();

private:
    Ui::CreateGroup *ui;
};

#endif // CREATEGROUP_H
