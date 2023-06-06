#include "task.h"
#include "ui_task.h"

#include <QInputDialog>

//-------------------------------------------------------------------------------------------------
task::task(const QString& name, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::task)
{
    ui->setupUi(this);
    set_name(name);

    connect(ui->pushButton_edit, &QPushButton::clicked, this, &task::rename);
    connect(ui->pushButton_remote, &QPushButton::clicked, [this, name] {
        qDebug() << "Trying to remove" << name;
        emit removed(this);
    });
    connect(ui->checkBox, &QCheckBox::toggled, this, &task::checked);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
//-------------------------------------------------------------------------------------------------
task::~task() {
    delete ui;
}
//-------------------------------------------------------------------------------------------------
void task::set_name(const QString &name) {
    ui->checkBox->setText(name);
}
//-------------------------------------------------------------------------------------------------
QString task::name() const {
    return ui->checkBox->text();
}
//-------------------------------------------------------------------------------------------------
bool task::is_completed() const {
    return ui->checkBox->isChecked();
}
//-------------------------------------------------------------------------------------------------
void task::rename() {
    bool ok;
    QString value = QInputDialog::getText(this,
                                          tr("Edit task"),
                                          tr("Task name"),
                                          QLineEdit::Normal,
                                          this->name(),
                                          &ok);

    if (ok && !value.isEmpty()) {
        set_name(value);
    }
}
//-------------------------------------------------------------------------------------------------
void task::checked(bool checked) {
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit status_changed(this);
}
//-------------------------------------------------------------------------------------------------
