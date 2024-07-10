#include "newlogin.h"
#include "ui_newlogin.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QString>
#include <QLineEdit>

newlogin::newlogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newlogin)
{
    ui->setupUi(this);
}

newlogin::~newlogin()
{
    delete ui;
}



void newlogin::on_pushButton_clicked()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://localhost:3000/saveUserData"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //  данные для отправки на сервер
    QString mail_str,sur_str,name_str;
    mail_str=ui->mailEdit->text();
    sur_str=ui->surEdit->text();
    name_str=ui->nameEdit->text();
    QJsonObject json;

    json["surname"] = sur_str;
    json["email"] = mail_str;
    json["nickname"] = name_str;

    qDebug() << mail_str;
    qDebug() << sur_str;
    qDebug() << name_str;


    QJsonDocument jsonData(json);
    QByteArray postData = jsonData.toJson();

    QNetworkReply *reply = manager.post(request, postData);

    QObject::connect(reply, &QNetworkReply::finished, [&](){
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Данные успешно отправлены";
            qDebug() << reply->readAll(); // Вывод ответа от сервера
        } else {
            qDebug() << "Ошибка при отправке данных:" << reply->errorString();
        }
        reply->deleteLater();
     });
}

