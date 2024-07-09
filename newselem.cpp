#include "newselem.h"
#include "ui_newselem.h"

NewsElem::NewsElem( int userID, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewsElem)
    , m_userID(userID)
    , networkManager(new NetworkManager(this))
{
    ui->setupUi(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &NewsElem::onHttpFinished);
}

NewsElem::~NewsElem()
{
    delete ui;
}

void NewsElem::on_pushButton_2_clicked()
{
    QString text = ui->postEdit->toPlainText();
    if(text.isEmpty()){
        qDebug() << "Message field is empty. Please write a message to send.";
        return;
    }

    ui->postEdit->clear();

    sendToServer(text);
}

void NewsElem::sendToServer(const QString& content)
{

    QUrl url("http://localhost:3000/posts/add");
    QNetworkRequest request(url);

    // Устанавливаем заголовок Content-Type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с командой и именем пользователя
    QJsonObject json;
    json["user_id"] = m_userID;
    json["content"] = content;


    // Отправляем POST запрос
    networkManager->post(request, QJsonDocument(json).toJson());
}

void NewsElem::onHttpFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

   // QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

    //qDebug() << "Received JSON:" << jsonDoc.toJson(QJsonDocument::Indented);

    // Получение массива из JSON-документа
    //QJsonArray jsonArray = jsonDoc.array();

    // Получение элемента массива по индексу
    //QJsonObject chat = jsonArray.at(0).toObject();

    //qDebug() << "Chat ID:" << chat["id"];
    //qDebug() << "Sender ID:" << chat["sender_id"];
    //qDebug() << "Receiver ID:" << chat["receiver_id"];
    //qDebug() << "Content:" << chat["content"];
    //qDebug() << "time:" << chat["created_at"];
    //on_newDataReceived(jsonArray);
}
