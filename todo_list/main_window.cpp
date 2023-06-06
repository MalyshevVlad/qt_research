#include "main_window.h"
#include "ui_main_window.h"

#include <QDebug>
#include <QInputDialog>
#include <QSizeGrip>

//-------------------------------------------------------------------------------------------------
main_window::main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_window),
    m_tasks()
{
    ui->setupUi(this);

    connect(ui->pushButton_add_task, &QPushButton::clicked, this, &main_window::add_task);
    update_status();

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QSizeGrip(ui->size_grip);
}
//-------------------------------------------------------------------------------------------------
main_window::~main_window() {
    delete ui;
}
//-------------------------------------------------------------------------------------------------
void main_window::add_task() {
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Add task"),
                                         tr("Task name"),
                                         QLineEdit::Normal,
                                         tr("Untitled task"),
                                         &ok);

    if (ok && !name.isEmpty()) {
        qDebug() << "Adding new task";
        task* new_task = new task(name);

        connect(new_task, &task::removed, this, &main_window::remove_task);
        connect(new_task, &task::status_changed, this, &main_window::task_status_changed);

        m_tasks.append(new_task);
        ui->layout_tasks->addWidget(new_task);

        update_status();
    }

}
//-------------------------------------------------------------------------------------------------
void main_window::remove_task(task* task) {
    m_tasks.removeOne(task);
    ui->layout_tasks->removeWidget(task);
    delete task;
    update_status();
}
//-------------------------------------------------------------------------------------------------
void main_window::task_status_changed(task* /*task*/) {
    update_status();
}
//-------------------------------------------------------------------------------------------------
void main_window::update_status()  {
    int completed_count = 0;

    for(auto task : m_tasks)  {
        if (task->is_completed()) {
            completed_count++;
        }
    }
    int todo_count = m_tasks.size() - completed_count;

    ui->label_status->setText(
            QString("Status: %1 todo / %2 completed").arg(todo_count).arg(completed_count)
        );
}
//-------------------------------------------------------------------------------------------------
