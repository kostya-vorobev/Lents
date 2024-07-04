#include "sendmessageuser.h"
#include "ui_sendmessageuser.h"

SendMessageUser::SendMessageUser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SendMessageUser)
{
    ui->setupUi(this);

    ui->TextL->setStyleSheet("QLabel { background-color : #e6e6e6; color : black; border-radius: 10px; }");
    ui->TimeL->setStyleSheet("QLabel { color : grey; }");
    this->setStyleSheet("SendMessageUser { margin:4px; }");
    // Меняем стили SendMessageUser
    this->setStyleSheet("SendMessageUser { border: 1px solid lightgrey; border-radius: 10px; background-color: #d1edf2; }");
}

SendMessageUser::~SendMessageUser()
{
    delete ui;
}

void SendMessageUser::setMessage(const QString& text)
{
    // Мы предполагаем, что у вас есть QLabel под названием MessageLabel в вашем пользовательском интерфейсе
    ui->TextL->setText(text);
}

void SendMessageUser::setTimestamp(const QString& timestamp)
{
    // Мы предполагаем, что у вас есть QLabel под названием TimestampLabel в пользовательском интерфейсе
    ui->TimeL->setText(timestamp);
}
