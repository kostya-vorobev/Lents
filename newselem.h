#ifndef NEWSELEM_H
#define NEWSELEM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <networkmanager.h>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class NewsElem;
}

class NewsElem : public QWidget
{
    Q_OBJECT

public:
    explicit NewsElem(int userID, QWidget *parent = nullptr);
    ~NewsElem();

private slots:
    void on_pushButton_2_clicked();
    void onHttpFinished(QNetworkReply *reply);
    void sendToServer(const QString& content);

private:
    Ui::NewsElem *ui;
    int m_userID;
    NetworkManager *networkManager;
};

#endif // NEWSELEM_H
