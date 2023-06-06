#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>

//-------------------------------------------------------------------------------------------------
namespace Ui { class task; }
//-------------------------------------------------------------------------------------------------
class task : public QWidget
{
    Q_OBJECT

private:
    Ui::task *ui;

public:
    explicit task(const QString& name, QWidget* parent = nullptr);
    ~task();

    void set_name(const QString& name);
    QString name() const;
    bool is_completed() const;

signals:
    void removed(task* task);
    void status_changed(task* task);

private slots:
    void checked(bool checked);

public slots:
    void rename();

};
//-------------------------------------------------------------------------------------------------
#endif // TASK_H
