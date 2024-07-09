#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>

#include <QNetworkReply>

class NetworkManager : public QNetworkAccessManager
{
public:
    NetworkManager(QObject *parent) : QNetworkAccessManager(parent) {}

protected:
    void finished(QNetworkReply *reply);
    void postSender(QNetworkReply *reply);
};

#endif // NETWORKMANAGER_H
