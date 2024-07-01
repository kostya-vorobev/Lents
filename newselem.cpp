#include "newselem.h"
#include "ui_newselem.h"

NewsElem::NewsElem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsElem)
{
    ui->setupUi(this);
}

NewsElem::~NewsElem()
{
    delete ui;
}
