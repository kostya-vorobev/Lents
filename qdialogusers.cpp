#include "qdialogusers.h"
#include "ui_qdialogusers.h"
#include <QThread>

QDialogUsers::QDialogUsers(QString username, QString usernameAuth, QWidget *parent)
    : QMainWindow(parent)
    , m_username(username)
    , m_usernameAuth(usernameAuth)
    , ui(new Ui::QDialogUsers)
    , networkManager(new NetworkManager(this))
{
    ui->setupUi(this);

    ui->OutputArea->setWidget(new QWidget());
    ui->OutputArea->widget()->setLayout(new QVBoxLayout());

    connect(networkManager, &QNetworkAccessManager::finished, this, &QDialogUsers::onHttpFinished);
    //networkManager->connectToHost("http://localhost", 3000);

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

    //sendToServer(text, timestamp, m_usernameAuth, m_username);
}

void QDialogUsers::sendToServer(const QString& message, const QString& timestamp, const QString& sender, const QString& receiver)
{
    /*QByteArray data;

    QString messageToSend = message;
    messageToSend.replace('\n', "<:LF:>");

    data.append("saveMessage\n");
    data.append(sender.toUtf8() + "\n");
    data.append(receiver.toUtf8() + "\n");
    data.append(messageToSend.toUtf8() + "\n");
    data.append(timestamp.toUtf8());

    m_socketManager->writeDataDialog(data);
    QScrollBar *scrollBar = ui->OutputArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());*/
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
    if (jsonArray.isEmpty()){
        qDebug() << "No data available for reading!";
        return;
    }


    // Разбиваем полученные данные на составные части
    for (int j =0 ; j< jsonArray.size(); j++)
    {
        QJsonObject chat = jsonArray.at(j).toObject();

        QString chatName = QString::number(chat["receiver_id"].toDouble());
        QString lastMessage = chat["content"].toString();
        QString messageTime = chat["created_at"].toString();
        QDateTime messageDateTime = QDateTime::fromString(messageTime, "yyyy-MM-ddTHH:mm:ss.zzzZ");

        if (maps.contains(messageDateTime))
            continue;
        // Добавляем информацию об этом чате в QMap
        maps[messageDateTime] = QPair<QString, QString>(chatName, lastMessage);



        SendMessageUser* newMessage = new SendMessageUser(this);
        newMessage->setMessage(lastMessage); // предполагаем, что само сообщение - это 3-е поле
        newMessage->setTimestamp(messageDateTime.toString("yyyy-MM-dd HH:mm:ss")); // устанавливаем временную метку

        QHBoxLayout* messageLayout = new QHBoxLayout();
        if(chatName == m_usernameAuth){
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
    QString urlStr = "http://localhost:3000/messages/conversation/"+ m_username +"/"+ m_usernameAuth;
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
    QJsonArray jsonArray = jsonDoc.array();

    // Получение элемента массива по индексу
    QJsonObject chat = jsonArray.at(1).toObject();

    qDebug() << "Chat ID:" << chat["id"];
    qDebug() << "Sender ID:" << chat["sender_id"];
    qDebug() << "Receiver ID:" << chat["receiver_id"];
    qDebug() << "Content:" << chat["content"];
    qDebug() << "time:" << chat["created_at"];
    on_newDataReceived(jsonArray);
}
