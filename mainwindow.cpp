#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем новый пост
    PostClass* post = new PostClass("AuthorName", "This is the post text", 0, 0);

    // Создаем новый виджет LentElem с этим постом
    LentElem* lentElem = new LentElem(post, this);

    // Получаем макет страницы LentPage
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->LentPage->layout());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

