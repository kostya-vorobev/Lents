#include "newlogin.h"
#include "ui_newlogin.h"


newlogin::newlogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newlogin)
    , networkManager(new NetworkManager(this))
{
    ui->setupUi(this);

    connect(networkManager, &QNetworkAccessManager::finished, this, &newlogin::onHttpFinished);
}

newlogin::~newlogin()
{
    delete ui;
}



void newlogin::on_pushButton_clicked()
{

    QUrl url("http://localhost:3000/users/add");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //  данные для отправки на сервер
    QString mail_str,sur_str,name_str;
    mail_str=ui->mailEdit->text();
    sur_str=ui->surEdit->text();
    name_str=ui->nameEdit->text();
    QJsonObject json;

    json["password"] = sur_str;
    json["email"] = mail_str;
    json["username"] = name_str;

    qDebug() << mail_str;
    qDebug() << sur_str;
    qDebug() << name_str;

    networkManager->post(request, QJsonDocument(json).toJson());

}

void newlogin::onHttpFinished(QNetworkReply *reply){
    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

    qDebug() << "Received JSON:" << jsonDoc.toJson(QJsonDocument::Indented);

    QJsonObject jsonObj;
    if (jsonDoc.isObject())
    {
        jsonObj = jsonDoc.object();
    }

    if(!jsonObj["id"].isNull())
    {
        MainWindow* mainWindow = new MainWindow(jsonObj["id"].toInt());
        mainWindow->show();
        this->close();
    }





}

