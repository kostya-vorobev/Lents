#ifndef SENDMESSAGEUSER_H
#define SENDMESSAGEUSER_H

#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QDebug>

namespace Ui {
class SendMessageUser;
}

class SendMessageUser : public QWidget
{
    Q_OBJECT

public:
    explicit SendMessageUser(QWidget *parent = nullptr);
    ~SendMessageUser();
    void setMessage(const QString& text);
    void setTimestamp(const QString& timestamp);


private:
    Ui::SendMessageUser *ui;
};

#endif // SENDMESSAGEUSER_H
