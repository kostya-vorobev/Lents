#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new NetworkManager(this))
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    m_username = "2";
    userID = 2;

    // Создаем 4 страницы в stackedWidget
    LentPage = ui->stackedWidget->widget(0);
    CreatePostPage = ui->stackedWidget->widget(1);
    TestsPage = ui->stackedWidget->widget(2);
    ChatPage = ui->stackedWidget->widget(3);

    // Устанавливаем связи между сигналами и слотами
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onHttpFinished);
    networkManager->connectToHost("http://localhost", 3000);
    checkAndUpdateChats();
    requestUserPosts();

    QWidget* chatPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    chatPage->setLayout(layout);

    NewsElem *newsElem = new NewsElem(userID, this);
    layout->addWidget(newsElem);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(chatPage);

    QVBoxLayout* layoutPage = new QVBoxLayout;
    layoutPage->addWidget(scrollArea);

    CreatePostPage->setLayout(layoutPage);


    ui->stackedWidget->setCurrentIndex(1); // Set current index to 0

    // Устанавливаем таймер, который будет вызывать requestUserChats каждые 10 секунд
    m_timer->setInterval(10000);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::checkAndUpdateChats);
    m_timer->start();
}

MainWindow::~MainWindow()
{
    delete networkManager;
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
    ui->updatePostB->show();
}


void MainWindow::on_pushButton_clicked()
{

    ui->stackedWidget->setCurrentIndex(3);
    ui->updatePostB->hide();
}

void MainWindow::addPost(int postID, const QString& author, const QString& text, int likes, int disLikes, QVBoxLayout* layout)
{
    // Создаем новый пост
    PostClass* post = new PostClass(postID, author, text, likes, disLikes);

    // Создаем новый виджет LentElem с этим постом
    LentElem* lentElem = new LentElem(post, this);

    // Добавляем виджет на страницу
    layout->addWidget(lentElem);
}

void MainWindow::updateLayout(QWidget* widget, QWidget* page)
{
    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    QVBoxLayout* layoutPage = new QVBoxLayout;
    layoutPage->addWidget(scrollArea);

    for (int i = 0; i < ui->stackedWidget->count(); ++i) {
        QWidget *page = ui->stackedWidget->widget(i);
        qDebug() << "Page " << i << ": " << (page ? page->objectName() : "nullptr");
    }
    qDeleteAll(page->children());
    page->setLayout(layoutPage);

}


void MainWindow::on_pushButton_4_clicked()
{
    ui->updatePostB->hide();
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::checkAndUpdateChats(){

        requestUserChats();
}

void MainWindow::onReadyRead(const QJsonArray &jsonArray) {

    QWidget* chatPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    chatPage->setLayout(layout);

    // Разбиваем полученные данные на составные части
    for (int j = 0 ; j < jsonArray.size(); j++)
     {
        QJsonObject chat = jsonArray.at(j).toObject();

        int user1_id = chat["user1_id"].toInt();
        QString user1_name = chat["user1_name"].toString();
        int user2_id = chat["user2_id"].toInt();
        QString user2_name = chat["user2_name"].toString();
        QString message = chat["message"].toString();
        QDateTime sent_at = QDateTime::fromString(chat["sent_at"].toString(), "yyyy-MM-ddTHH:mm:ss.zzzZ");

        ChatClass* chatUnit = new ChatClass(userID, user1_id, user1_name, user2_id, user2_name, message, sent_at);
        UserChatButton *chatButton = createChatButton(chatUnit);
        // Добавляем чат-кнопку в макет
        layout->addWidget(chatButton);
    }

    updateLayout(chatPage, ChatPage);
}

void MainWindow::onReadyReadPosts(const QJsonArray &jsonArray) {

    QWidget* postPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    postPage->setLayout(layout);

    // Разбиваем полученные данные на составные части
    for (int j = 0 ; j < jsonArray.size(); j++)
    {
        QJsonObject post = jsonArray.at(j).toObject();
        int postID = post["id"].toInt();
        int likes = post["like_count"].toInt();
        int disLikes = post["dislike_count"].toInt();
        QString PostName = post["username"].toString();
        QString content = post["content"].toString();
        QString createdTime = post["created_at"].toString();
        //qDebug()<< createdTime;
        //qDebug()<< PostName;
        //qDebug()<< content;
        QDateTime messageDateTime = QDateTime::fromString(createdTime, "yyyy-MM-ddTHH:mm:ss.zzzZ");
        // Добавляем информацию об этом чате в QMap
        PostClass* postUnit = new PostClass(postID, PostName, content, likes, disLikes);
        LentElem* lentElem = new LentElem(postUnit, this);
        layout->addWidget(lentElem);

    }

    updateLayout(postPage, LentPage);
}



UserChatButton* MainWindow::createChatButton(ChatClass* chatUnit) {
    UserChatButton *chatButton = new UserChatButton(chatUnit, this);
    chatButton->setStyleSheet(
        "QWidget {"
        "background-color: #f4f4f4;"
        "border: 1px solid #ccc;"
        "border-radius: 5px;"
        "color: #333;"
        "}"
        "QWidget:hover {"
        "background-color: #dcdcdc;"
        "}"

        );
    chatButton->setLastMessage(chatUnit->getMessage());
    chatButton->setLastMessageTime(chatUnit->getSentAt());
    connect(chatButton, &UserChatButton::clicked, chatButton, &UserChatButton::onClicked);
    chatButton->setFixedHeight(100); // задаем фиксированную высоту для кнопки

    return chatButton;
}

void MainWindow::requestUserChats() {
    QString urlStr = "http://localhost:3000/messages/last-message/"+ QString::number(userID);
    QUrl url(urlStr); // Укажите правильный URL вашего сервера
    QNetworkRequest request(url);

    // Устанавливаем заголовок Content-Type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с командой и именем пользователя
    QJsonObject json;

    // Отправляем POST запрос
    networkManager->get(request, QJsonDocument(json).toJson());


}

void MainWindow::requestUserPosts() {
    QString urlStr = "http://localhost:3000/posts/user/"+ QString::number(userID);
    QUrl url(urlStr); // Укажите правильный URL вашего сервера
    QNetworkRequest request(url);

    // Устанавливаем заголовок Content-Type
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Создаем JSON объект с командой и именем пользователя
    QJsonObject json;

    // Отправляем POST запрос
    networkManager->get(request, QJsonDocument(json).toJson());


}

void MainWindow::onHttpFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());

    //qDebug() << "Received JSON:" << jsonDoc.toJson(QJsonDocument::Indented);

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

    //QJsonObject jsonObjs = jsonArray.at(0).toObject();
    if(jsonObj["tag"] == "messages")
    {
        qDebug() <<  jsonObj;
        QJsonArray jsonArray = jsonObj["data"].toArray();
        qDebug() << jsonArray.size();
        if (jsonArray.size() > 0){

            qDebug() << jsonObj["data"];
            //qDebug() << jsonObj;
            onReadyRead(jsonArray);
        }

        onReadyRead(jsonArray);
    }
    if(jsonObj["tag"] == "posts")
    {
        QJsonArray jsonArray = jsonObj["data"].toArray();
        if (jsonArray.size() > 0){

            //qDebug() << jsonObj["data"];
            //qDebug() << jsonObj;
            onReadyReadPosts(jsonArray);
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->updatePostB->hide();
}


void MainWindow::on_updatePostB_clicked()
{
    requestUserPosts();
}

