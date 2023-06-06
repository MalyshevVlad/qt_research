#ifndef CONFIRMWINDOW_H
#define CONFIRMWINDOW_H

#include <QWidget>

namespace Ui {
class ConfirmWindow;
}

class ConfirmWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfirmWindow(QWidget *parent = nullptr);
    ~ConfirmWindow();

private:
    Ui::ConfirmWindow *ui;
};

#endif // CONFIRMWINDOW_H
