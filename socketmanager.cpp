// socketmanager.cpp

#include "socketmanager.h"
#include <QThread>
#include <QDebug>

SocketManager::SocketManager(QObject *parent)
    : QObject(parent)
    , m_tcpSocket(new QTcpSocket(this))
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &SocketManager::connected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &SocketManager::disconnected);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &SocketManager::handleNetworkError);
    connect(m_tcpSocket, &QTcpSocket::readyRead, [=]() {
        emit dataReceived(m_tcpSocket->readAll());
    });
}

void SocketManager::connectToServer()
{
    if(m_tcpSocket->state() != QAbstractSocket::ConnectedState){
        m_tcpSocket->connectToHost("localhost", 4242);
    }
}



void SocketManager::handleNetworkError(QAbstractSocket::SocketError socketError){
    qDebug() << "Network Error: " << m_tcpSocket->errorString();
    QThread::sleep(1);
    connectToServer();
}

void SocketManager::writeData(const QString &command, const QString &message){
    QString data = QString("%1\n%2\n").arg(command, message);
    if(m_tcpSocket->state() == QAbstractSocket::ConnectedState){
        m_tcpSocket->write(data.toUtf8());
        m_tcpSocket->flush();
    } else {
        qDebug() << "Error: Attempt to write data on a non-active socket";
    }
}


void SocketManager::writeDataDialog(const QByteArray &data){
    if(m_tcpSocket->state() == QAbstractSocket::ConnectedState){
        m_tcpSocket->write(data);
    } else {
        qDebug() << "Error: Attempt to write data on a non-active socket";
    }
}
