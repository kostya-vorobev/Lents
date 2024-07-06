#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new NetworkManager(this))
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    m_username = "Kostya";
    // Добавляем несколько постов
    QWidget* lentPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    lentPage->setLayout(layout);
    for(int i = 0; i < 10; i++)
    {
        addPost("AuthorName", "This is the post text", i+1, i, layout);
    }

    // Обновляем layout
    updateLayout(lentPage);

    ui->stackedWidget->setCurrentIndex(0);

    // Устанавливаем связи между сигналами и слотами
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onHttpFinished);
    networkManager->connectToHost("http://localhost", 3000);
    checkAndUpdateChats();
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
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::addPost(const QString& author, const QString& text, int likes, int comments, QVBoxLayout* layout)
{
    // Создаем новый пост
    PostClass* post = new PostClass(author, text, likes, comments);

    // Создаем новый виджет LentElem с этим постом
    LentElem* lentElem = new LentElem(post, this);

    // Добавляем виджет на страницу
    layout->addWidget(lentElem);
}

void MainWindow::updateLayout(QWidget* widget)
{
    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    // Очищаем текущее содержимое layout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(0)) ) {
        delete item->widget();
        delete item;
    }

    // Добавляем новый QScrollArea в layout
    ui->stackedWidget->layout()->addWidget(scrollArea);
}


void MainWindow::on_pushButton_4_clicked()
{
    // Добавляем несколько постов
    QWidget* createPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    createPage->setLayout(layout);

    // Создаем новый виджет LentElem с этим постом
    NewsElem* newsElem = new NewsElem(this);

    // Добавляем виджет на страницу
    layout->addWidget(newsElem);

    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(createPage);

    // Очищаем текущее содержимое layout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(2)) ) {
        delete item->widget();
        delete item;
    }

    // Добавляем новый QScrollArea в layout
    ui->stackedWidget->layout()->addWidget(scrollArea);

    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::checkAndUpdateChats(){
        requestUserChats();
}

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

    // Создаем новый QScrollArea и устанавливаем его виджетом
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(chatPage);

    // Очищаем текущее содержимое layout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(2)) ) {
        delete item->widget();
        delete item;
    }

    // Добавляем новый QScrollArea в layout
    ui->stackedWidget->layout()->addWidget(scrollArea);

}


UserChatButton* MainWindow::createChatButton(const QString &chatName, const QString &lastMessage, const QDateTime &messageTime) {
    UserChatButton *chatButton = new UserChatButton(chatName, m_username);
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
    chatButton->setLastMessage(lastMessage);
    chatButton->setLastMessageTime(messageTime);
    connect(chatButton, &UserChatButton::clicked, chatButton, &UserChatButton::onClicked);
    chatButton->setFixedHeight(100); // задаем фиксированную высоту для кнопки

    return chatButton;
}

void MainWindow::updateChatLayout(QWidget *widget) {
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    // Clear current content of ChatLayout
    QLayoutItem* item;
    while ( (item = ui->stackedWidget->layout()->takeAt(2)) ) {
        delete item->widget();
        delete item;
    }

    ui->stackedWidget->addWidget(scrollArea);
}

void MainWindow::requestUserChats() {
    QUrl url("http://localhost:3000/messages/conversation/1/2"); // Укажите правильный URL вашего сервера
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
    onReadyRead(jsonArray);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

