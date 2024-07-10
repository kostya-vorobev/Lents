#ifndef NEWLOGIN_H
#define NEWLOGIN_H

#include <QDialog>

namespace Ui {
class newlogin;
}

class newlogin : public QDialog
{
    Q_OBJECT

public:
    explicit newlogin(QWidget *parent = nullptr);
    ~newlogin();

private slots:

    void on_pushButton_clicked();

private:
    Ui::newlogin *ui;
};

#endif // NEWLOGIN_H
