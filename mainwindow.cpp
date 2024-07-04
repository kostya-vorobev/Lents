#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    connect(socketManager, SIGNAL(dataReceived(QByteArray)), this, SLOT(onReadyRead(QByteArray)));
    connect(socketManager, SIGNAL(connected()), this, SLOT(onConnected()));

    // Пытаемся подключиться к серверу при инициализации
    socketManager->connectToServer();

    // Устанавливаем таймер, который будет вызывать requestUserChats каждые 10 секунд
    m_timer->setInterval(10000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkAndUpdateChats()));
    m_timer->start();
}

MainWindow::~MainWindow()
{
    delete socketManager;
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
    // Если поле поиска не пустое, не обновляем список чатов, иначе обновляем
    if(ui->SearchChatTB->toPlainText().isEmpty()){
        requestUserChats();
    }
}



HolodWindow::~HolodWindow()
{
    delete socketManager;
    delete ui;
}

void HolodWindow::onConnected()
{
    // Когда подключились к серверу, делаем запрос на чаты
    requestUserChats();
}

// Объект socketManager отправляет команду и соответствующее сообщение серверу.
void HolodWindow::sendCommand(const QString &command, const QString &message) {
    socketManager->writeData(command, message);
}

// Эта функция выполняется при получении данных от сервера
void HolodWindow::onReadyRead(const QByteArray &receivedData) {
    // Выводим имя пользователя в консоль
    qDebug() << m_username;
    // Создаем новый виджет и макет для чатов
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    // Устанавливаем белый цвет фона виджета
    scrollWidget->setStyleSheet("background-color: #fff;");
    // Устанавливаем выравнивание элементов макета по верхнему краю
    scrollLayout->setAlignment(Qt::AlignTop);

    // Разбиваем полученные данные на составные части
    QStringList responseParts = QString(receivedData).split("\n");

    // Сохраняем все чаты в QMap
    QMap<QDateTime, QPair<QString, QString>> maps;

    // Итерируемся по каждому чату в полученных данных
    for (const QString &chatInfo : responseParts) {
        // Пропускаем пустые имена
        if (chatInfo.trimmed().isEmpty())
            continue;

        // Разбиваем информацию чата на имя пользователя и сообщение
        QStringList chatParts = chatInfo.split("|");
        QString chatName = chatParts[0].trimmed();
        QString lastMessage = chatParts.size() > 2 ? chatParts[1] : "Нет сообщений";
        QString messageTime = chatParts.size() > 2 ? chatParts[2] : "";
        if (chatName == "") chatName = m_username;

        // Добавляем информацию об этом чате в QMap
        maps[QDateTime::fromString(messageTime, "yyyy/MM/dd HH:mm:ss")] = QPair<QString, QString>(chatName, lastMessage);
    }

    // Преобразуем QMap в map из стандартной библиотеки C++
    std::map<QDateTime, QPair<QString, QString>> stdMap = maps.toStdMap();

    // Итерируемся через map и создаем кнопку для каждого чата
    for(auto i = stdMap.rbegin(); i != stdMap.rend(); i++) {
        UserChatButton *chatButton = createChatButton(i->second.first, i->second.second, i->first);
        // Добавляем чат-кнопку в макет
        scrollLayout->addWidget(chatButton);
    }

    // Обновляем макет чата с новыми виджетами-кнопками
    updateChatLayout(scrollWidget);
}

UserChatButton* HolodWindow::createChatButton(const QString &chatName, const QString &lastMessage, const QDateTime &messageTime) {
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

void HolodWindow::updateSearch(const QString &newText) {
    // Если поле поиска не пустое, отправляем запрос на поиск на сервер.
    if(!newText.isEmpty()){
        sendCommand("search", newText);
    } else {
        // Если поле поиска снова стало пустым, мы обновляем список чатов
        requestUserChats();
    }
}

void HolodWindow::updateChatLayout(QWidget *widget) {
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    // Clear current content of ChatLayout
    QLayoutItem* item;
    while ( (item = ui->ChatLayout->takeAt(0)) ) {
        delete item->widget();
        delete item;
    }

    ui->ChatLayout->addWidget(scrollArea);
}

void HolodWindow::requestUserChats() {
    sendCommand("getChats", m_username);
}


void HolodWindow::on_SearchChatTB_textChanged()
{
    updateSearch(ui->SearchChatTB->toPlainText());
}

