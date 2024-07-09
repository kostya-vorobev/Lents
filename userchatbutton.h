#ifndef USERCHATBUTTON_H
#define USERCHATBUTTON_H

#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <qdialogusers.h>
#include <chatclass.h>

namespace Ui {
class UserChatButton;
}

class UserChatButton : public QWidget
{
    Q_OBJECT

public:
    explicit UserChatButton(ChatClass* chatUnit, QWidget* parent = nullptr);
    ~UserChatButton();

    void setUsername(const QString& username);
    void setLastMessage(const QString& message);
    void setLastMessageTime(const QDateTime& timestamp);

signals:
    void clicked();

public slots:
    void onClicked();


protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::UserChatButton *ui;
    QString m_username;
    QDialogUsers *m_dialogUsers = nullptr;
    ChatClass* chatUnit;
};

#endif // USERCHATBUTTON_H
