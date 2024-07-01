#include "lentelem.h"
#include "ui_lentelem.h"

LentElem::LentElem(PostClass* post, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LentElem)
    , post(post)
{
    ui->setupUi(this);
    ui->AuthorL->setText(QString::fromStdString(post->getAuthor()));
    ui->TextPostL->setText(QString::fromStdString(post->getPostText()));
    ui->LikeL->setText(QString::number(post->getCountLike()));
    ui->DislikeL->setText(QString::number(post->getCountDislike()));
}

LentElem::~LentElem()
{
    delete ui;
}

void LentElem::on_pushButton_2_clicked()
{
    post->incrementLikes();
    ui->LikeL->setText(QString::number(post->getCountLike()));
}

void LentElem::on_pushButton_clicked()
{
    post->incrementDislikes();
    ui->DislikeL->setText(QString::number(post->getCountDislike()));
}
