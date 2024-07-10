#ifndef AUTH_H
#define AUTH_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class Auth;
}
QT_END_NAMESPACE

class Auth : public QMainWindow
{
    Q_OBJECT

public:
    Auth(QWidget *parent = nullptr);
    ~Auth();

private slots:
    void on_tgLabel_linkActivated(const QString &link);

    void on_pushButton_clicked();

private:
    Ui::Auth *ui;
};
#endif // MAINWINDOW_H
