#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QFrame {
    Q_OBJECT

private:
    Ui::AboutDialog *ui; // в публике

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();
};

#endif // ABOUTDIALOG_H
