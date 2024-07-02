#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Добавляем несколько постов
    QWidget* lentPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    lentPage->setLayout(layout);
    for(int i = 0; i < 10; i++)
    {
        addPost("AuthorName", "This is the post text", i+1, i, layout);
    }

    // Обновляем layout
    updateLayout(lentPage);

    ui->stackedWidget->setCurrentIndex(0);

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

void MainWindow::addPost(const QString& author, const QString& text, int likes, int comments, QVBoxLayout* layout)
{
    // Создаем новый пост
    PostClass* post = new PostClass(author, text, likes, comments);

    // Создаем новый виджет LentElem с этим постом
    LentElem* lentElem = new LentElem(post, this);

    // Добавляем виджет на страницу
    layout->addWidget(lentElem);
}

void MainWindow::updateLayout(QWidget* widget)
{
    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    // Очищаем текущее содержимое layout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(0)) ) {
        delete item->widget();
        delete item;
    }

    // Добавляем новый QScrollArea в layout
    ui->stackedWidget->layout()->addWidget(scrollArea);
}


void MainWindow::on_pushButton_4_clicked()
{
    // Добавляем несколько постов
    QWidget* createPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    createPage->setLayout(layout);

    // Создаем новый виджет LentElem с этим постом
    NewsElem* newsElem = new NewsElem(this);

    // Добавляем виджет на страницу
    layout->addWidget(newsElem);

    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(createPage);

    // Очищаем текущее содержимое layout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(2)) ) {
        delete item->widget();
        delete item;
    }

    // Добавляем новый QScrollArea в layout
    ui->stackedWidget->layout()->addWidget(scrollArea);

    ui->stackedWidget->setCurrentIndex(2);
}

