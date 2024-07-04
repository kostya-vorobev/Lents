#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>

class SocketManager : public QObject
{
    Q_OBJECT
public:
    explicit SocketManager(QObject *parent = 0);
    void connectToServer();
    void handleNetworkError(QAbstractSocket::SocketError socketError);
    void writeData(const QString &command, const QString &message);
    void writeDataDialog(const QByteArray &data);



signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray &data);
    void serverConnected();


private:
    QTcpSocket *m_tcpSocket;

};

#endif // SOCKETMANAGER_H
