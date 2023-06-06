#ifndef OUTPUT_ORM_H
#define OUTPUT_ORM_H

#include <QWidget>

namespace Ui {
class output_orm;
}

class output_orm : public QWidget
{
    Q_OBJECT

public:
    explicit output_orm(QWidget *parent = nullptr);
    ~output_orm();

private:
    Ui::output_orm *ui;
};

#endif // OUTPUT_ORM_H
