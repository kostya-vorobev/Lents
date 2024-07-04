#include "qdialogusers.h"
#include "ui_qdialogusers.h"
#include <QThread>
#include "socketmanager.h"

QDialogUsers::QDialogUsers(QString username, QString usernameAuth, QWidget *parent)
    : QMainWindow(parent)
    , m_username(username)
    , m_usernameAuth(usernameAuth)
    , ui(new Ui::QDialogUsers)
    , m_socketManager(new SocketManager(this))
{
    ui->setupUi(this);

    ui->OutputArea->setWidget(new QWidget());
    ui->OutputArea->widget()->setLayout(new QVBoxLayout());

    connect(m_socketManager, &SocketManager::connected, this, &QDialogUsers::on_socketConnected);
    connect(m_socketManager, &SocketManager::dataReceived, this, &QDialogUsers::on_newDataReceived);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QDialogUsers::requestUpdatedData);
    timer->start(2000);

    ui->OutputArea->setStyleSheet("background-color: #fff;");
    m_socketManager->connectToServer();


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

    sendToServer(text, timestamp, m_usernameAuth, m_username);

}

void QDialogUsers::on_socketConnected()
{
    qDebug() << "Socket connected, ready to send data";
    requestUpdatedData();
}

void QDialogUsers::sendToServer(const QString& message, const QString& timestamp, const QString& sender, const QString& receiver)
{
    QByteArray data;

    QString messageToSend = message;
    messageToSend.replace('\n', "<:LF:>");

    data.append("saveMessage\n");
    data.append(sender + "\n");
    data.append(receiver + "\n");
    data.append(messageToSend + "\n");
    data.append(timestamp);

    m_socketManager->writeDataDialog(data);
    QScrollBar *scrollBar = ui->OutputArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void QDialogUsers::requestUpdatedData()
{
    qDebug() << "Requesting updated data...";

    QByteArray data;
    data.append("checkNewMessages\n");
    data.append(m_usernameAuth + "\n");
    data.append(m_username + "\n");  // Добавьте пользователя сообщения
    data.append(m_lastUpdateTime.toString("yyyy/MM/dd HH:mm:ss"));
    qDebug() << m_lastUpdateTime.toString();
    m_socketManager->writeDataDialog(data);
}


void QDialogUsers::on_newDataReceived(const QByteArray& data)
{
    if (data.isEmpty()){
        qDebug() << "No data available for reading!";
        return;
    }

    QStringList messageParts = QString::fromUtf8(data).split('\n', Qt::SkipEmptyParts);
    qDebug() << messageParts;
   /* if (!messageParts.isEmpty())
        messageParts.removeFirst();*/
    qDebug() << messageParts;
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
    scrollBar->setValue(scrollBar->maximum());

}
