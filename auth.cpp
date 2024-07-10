#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newlogin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/reso/img/tg.png");
    int w,h;
    w = ui-> tgLabel->width();
    h = ui-> tgLabel->height();
    ui->tgLabel->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    this->setStyleSheet("background-color: #454445;"); // Устанавливаем цвет фона формы

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tgLabel_linkActivated(const QString &link)
{

}


void MainWindow::on_pushButton_clicked()
{
    newlogin log;
    log.setModal(true);
    log.exec();


}

