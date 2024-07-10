#include "auth.h"
#include "ui_auth.h"
#include "newlogin.h"

Auth::Auth(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Auth)
{
    ui->setupUi(this);
    QPixmap pix(":/reso/img/tg.png");
    int w,h;
    w = ui-> tgLabel->width();
    h = ui-> tgLabel->height();
    ui->tgLabel->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    this->setStyleSheet("background-color: #454445;"); // Устанавливаем цвет фона формы

}


Auth::~Auth()
{
    delete ui;
}

void Auth::on_tgLabel_linkActivated(const QString &link)
{

}


void Auth::on_pushButton_clicked()
{
    newlogin log;
    log.setModal(true);
    log.exec();


}

