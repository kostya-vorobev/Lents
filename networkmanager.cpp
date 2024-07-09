#include "networkmanager.h"

//NetworkManager::NetworkManager(QObject *parent) : QNetworkAccessManager(parent) {}

void NetworkManager::finished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Error: " << reply->errorString();
    } else {
        QByteArray buffer;
        buffer.append(reply->readAll());
        QString response = QString::fromUtf8(buffer);
        qDebug() << "Response: " << response; // Display the response in the console
    }

    reply->deleteLater();
}

void NetworkManager::postSender(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Error: " << reply->errorString();
    } else {
        QByteArray buffer;
        buffer.append(reply->readAll());
        QString response = QString::fromUtf8(buffer);
        qDebug() << "Response: " << response; // Display the response in the console
    }

    reply->deleteLater();
}

