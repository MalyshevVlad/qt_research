#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVector>

#include "task.h"

//-------------------------------------------------------------------------------------------------
QT_BEGIN_NAMESPACE
namespace Ui { class main_window; }
QT_END_NAMESPACE
//-------------------------------------------------------------------------------------------------
class main_window : public QMainWindow
{
    Q_OBJECT
private:
    Ui::main_window* ui;
    QVector<task*> m_tasks;

public:
    main_window(QWidget* parent = nullptr);
    ~main_window();
    void update_status();

public slots:
    void add_task();
    void remove_task(task* task);
    void task_status_changed(task* task);

};
//-------------------------------------------------------------------------------------------------
#endif // MAIN_WINDOW_H
