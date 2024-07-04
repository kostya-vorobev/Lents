#include "userchatbutton.h"
#include "ui_userchatbutton.h"
#include <QMouseEvent>

UserChatButton::UserChatButton(QString username, QString usernameAuth, QWidget* parent) :
    QWidget(parent),
    m_username(usernameAuth),
    ui(new Ui::UserChatButton)
{
    ui->setupUi(this);
    setUsername(username);
    connect(this, &UserChatButton::clicked, this, &UserChatButton::onClicked);
    ui->UsernameL->setStyleSheet("QLabel {"
                                 "font: bold 14px;"
                                 "color: #333;"
                                 "}");

    // Установка стиля для TimeL
    ui->TimeL->setStyleSheet("QLabel {"
                             "font: 10px;"
                             "color: #888;"
                             "}");

    // Установка стиля для LastMessageL
    ui->LastMessageL->setStyleSheet("QLabel {"
                                    "color: #333;"
                                    "}");
}

UserChatButton::~UserChatButton()
{
    delete ui;
}

void UserChatButton::setUsername(const QString& username) {
    ui->UsernameL->setText(username);
}

void UserChatButton::setLastMessage(const QString& message) {
    ui->LastMessageL->setText(message);
}

void UserChatButton::setLastMessageTime(const QDateTime& timestamp) {
    ui->TimeL->setText(timestamp.time().toString());
}

void UserChatButton::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void UserChatButton::onClicked()
{
    qDebug() << "Chat button clicked for user: " << ui->UsernameL->text();

    if (!m_dialogUsers) { // Если окно не открыто
        m_dialogUsers = new QDialogUsers(ui->UsernameL->text(), m_username);
        // Удалите окно при закрытии
        connect(m_dialogUsers, &QDialogUsers::destroyed, [this] { m_dialogUsers = nullptr; });
    }

    m_dialogUsers->raise(); // Перемещаем окно на передний план
    m_dialogUsers->activateWindow(); // Активируем окно
    m_dialogUsers->show(); // Отображаем окно
}
