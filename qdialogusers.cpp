#include "qdialogusers.h"
#include "ui_qdialogusers.h"
#include <QThread>

QDialogUsers::QDialogUsers(ChatClass* chatUnit, QWidget *parent)
    : QMainWindow(parent)
    , m_username(chatUnit->getUser1Name())
    , m_usernameAuth(chatUnit->getUser2Name())
    , ui(new Ui::QDialogUsers)
    , networkManager(new NetworkManager(this))
{
    ui->setupUi(this);
    this->chatUnit = chatUnit;
    ui->OutputArea->setWidget(new QWidget());
    ui->OutputArea->widget()->setLayout(new QVBoxLayout());

    connect(networkManager, &QNetworkAccessManager::finished, this, &QDialogUsers::onHttpFinished);
    //networkManager->connectToHost("http://localhost", 3000);
    requestUpdatedData();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QDialogUsers::requestUpdatedData);
    timer->start(2000);

    ui->OutputArea->setStyleSheet("background-color: #fff;");

}

QDialogUsers::~QDialogUsers()
{
    delete ui;
}

void QDialogUsers::on_pushButton_clicked()
{
    QString text = ui->SendMessageTB->toPlainText();
    if(text.isEmpty()){
        qDebug() << "Message field is empty. Please write a message to send.";
        return;
    }

    SendMessageUser* newMessage = new SendMessageUser(this);
    newMessage->setMessage(text);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
    newMessage->setTimestamp(timestamp);
    qDebug() << timestamp;

    ui->SendMessageTB->clear();
    if(chatUnit->getId() == chatUnit->getUser1Id())
    {
        sendToServer(text, QString::number(chatUnit->getId()), QString::number(chatUnit->getUser2Id()));
    }else
    {
        sendToServer(text, QString::number(chatUnit->getId()), QString::number(chatUnit->getUser1Id()));
    }
}

void QDialogUsers::sendToServer(const QString& message, const QString& sender, const QString& receiver)
{

    QUrl url("http://localhost:3000/messages/send");
    QNetworkRequest request(url);

    // Устанавливаем заголовок Content-Type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с командой и именем пользователя
    QJsonObject json;
    json["content"] = message;
    json["sender_id"] = sender;
    json["receiver_id"] = receiver;

    // Отправляем POST запрос
    networkManager->post(request, QJsonDocument(json).toJson());
}

/*
void MainWindow::onReadyRead(const QJsonArray &jsonArray) {
    QMap<QDateTime, QPair<QString, QString>> maps;
    // Разбиваем полученные данные на составные части
    for (int j =0 ; j< jsonArray.size(); j++)
    {
        QJsonObject chat = jsonArray.at(j).toObject();

        QString chatName = QString::number(chat["receiver_id"].toDouble());
        QString lastMessage = chat["content"].toString();
        QString messageTime = chat["created_at"].toString();
        QDateTime messageDateTime = QDateTime::fromString(messageTime, "yyyy-MM-ddTHH:mm:ss.zzzZ");
        // Добавляем информацию об этом чате в QMap
        maps[messageDateTime] = QPair<QString, QString>(chatName, lastMessage);
    }

    // Преобразуем QMap в map из стандартной библиотеки C++
    std::map<QDateTime, QPair<QString, QString>> stdMap = maps.toStdMap();
    qDebug() << stdMap;
    QWidget* chatPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    chatPage->setLayout(layout);

    // Итерируемся через map и создаем кнопку для каждого чата
    for(auto i = stdMap.rbegin(); i != stdMap.rend(); i++) {
        UserChatButton *chatButton = createChatButton(i->second.first, i->second.second, i->first);
        // Добавляем чат-кнопку в макет
        layout->addWidget(chatButton);
    }

    updateLayout(chatPage, 2);
}*/

void QDialogUsers::on_newDataReceived(const QJsonArray &jsonArray)
{
    qDebug() << "1";
    if (jsonArray.isEmpty()){
        qDebug() << "No data available for reading!";
        return;
    }


    // Разбиваем полученные данные на составные части
    for (int j =0 ; j< jsonArray.size(); j++)
    {

        QJsonObject chat = jsonArray.at(j).toObject();
        int receiver_id = chat["receiver_id"].toInt();
        QString chatName = QString::number(chat["receiver_id"].toInt());
        QString lastMessage = chat["content"].toString();
        QString messageTime = chat["sent_at"].toString();
        QDateTime messageDateTime = QDateTime::fromString(messageTime, "yyyy-MM-ddTHH:mm:ss.zzzZ");

        if (maps.contains(messageDateTime))
            continue;
        // Добавляем информацию об этом чате в QMap
        maps[messageDateTime] = QPair<QString, QString>(chatName, lastMessage);



        SendMessageUser* newMessage = new SendMessageUser(this);
        newMessage->setMessage(lastMessage); // предполагаем, что само сообщение - это 3-е поле
        newMessage->setTimestamp(messageDateTime.toString("yyyy-MM-dd HH:mm:ss")); // устанавливаем временную метку

        QHBoxLayout* messageLayout = new QHBoxLayout();
        qDebug() << chatUnit->getId();
        qDebug() << receiver_id;
        if(chatUnit->getId() != receiver_id){
            messageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
            messageLayout->addWidget(newMessage);
        }
        else {
            messageLayout->addWidget(newMessage);
            messageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        }

        QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->OutputArea->widget()->layout());

        if(!layout) {
            qDebug() << "Layout is not QVBoxLayout";
            return;
        }

        layout->addLayout(messageLayout);
        layout->update();
    }

    QScrollBar *scrollBar = ui->OutputArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());

/*
    for (const QString &message : messageParts) {
        qDebug() << "Processing message:" << message;

        QStringList messageDetails = message.split('|'); // | - предполагаемый разделитель в строке сообщения
        if (messageDetails.size() < 3) continue; // Если сообщение не полное, то пропустите его
        QString messageText = messageDetails.at(1);
        messageText.replace("<:LF:>", "\n"); // замена обратно на \n
        QString timestampStr = messageDetails.at(2); // Предполагаем, что временная метка - 4-е поле в сообщении
        QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy/MM/dd HH:mm:ss");
        if (timestamp > m_lastUpdateTime) m_lastUpdateTime = timestamp;
        qDebug() << m_lastUpdateTime.toString();
        SendMessageUser* newMessage = new SendMessageUser(this);
        newMessage->setMessage(messageText); // предполагаем, что само сообщение - это 3-е поле
        newMessage->setTimestamp(timestampStr); // устанавливаем временную метку

        QHBoxLayout* messageLayout = new QHBoxLayout();
        if(messageDetails.at(0) == m_usernameAuth){
            messageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
            messageLayout->addWidget(newMessage);
        }
        else {
            messageLayout->addWidget(newMessage);
            messageLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        }

        QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->OutputArea->widget()->layout());

        if(!layout) {
            qDebug() << "Layout is not QVBoxLayout";
            return;
        }

        layout->addLayout(messageLayout);
        layout->update();
    }
    QScrollBar *scrollBar = ui->OutputArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());*/
}

void QDialogUsers::requestUpdatedData() {

    QString userID1 = QString::number(chatUnit->getUser1Id());
    QString userID2 = QString::number(chatUnit->getUser2Id());
    QString urlStr = "http://localhost:3000/messages/conversation/"+ userID1 +"/"+userID2;

    qDebug() << urlStr;
    QUrl url(urlStr); // Укажите правильный URL вашего сервера
    QNetworkRequest request(url);

    // Устанавливаем заголовок Content-Type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с командой и именем пользователя
    QJsonObject json;

    // Отправляем POST запрос
    networkManager->get(request, QJsonDocument(json).toJson());


}

void QDialogUsers::onHttpFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

    qDebug() << "Received JSON:" << jsonDoc.toJson(QJsonDocument::Indented);

    // Получение массива из JSON-документа

    //QJsonObject jsonObjs = jsonArray.at(0).toObject();
    //qDebug() << jsonObjs;
    //QJsonArray jsonArray1 = jsonArray[1];

    QJsonObject jsonObj;
    if (jsonDoc.isObject())
    {
        jsonObj = jsonDoc.object();
        //jsonArray.append(jsonObj);
    }


    if(jsonObj["tag"] == "messages")
    {
        QJsonArray jsonArray = jsonObj["data"].toArray();
        if (jsonArray.size() > 0){

            //qDebug() << jsonObj["data"];
            //qDebug() << jsonObj;
            on_newDataReceived(jsonArray);
        }
    }
}
