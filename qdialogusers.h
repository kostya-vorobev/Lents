#include <QMainWindow>
#include <QVBoxLayout>
#include "socketmanager.h"
#include "sendmessageuser.h"
#include <QQueue>
#include <QTimer>
#include <QScrollBar>


namespace Ui
{
class QDialogUsers;
}

class QDialogUsers : public QMainWindow
{
    Q_OBJECT

public:
    explicit QDialogUsers(QString username, QString usernameAuth, QWidget *parent = nullptr);
    ~QDialogUsers();

private slots:
    void on_pushButton_clicked();
    void on_socketConnected();
    void on_newDataReceived(const QByteArray& data);
    void requestUpdatedData();

private:
    void sendToServer(const QString& message, const QString& timestamp, const QString& sender, const QString& receiver);

    QString m_username;
    QString m_usernameAuth;
    QQueue<QByteArray> m_writeQueue;
    Ui::QDialogUsers *ui;
    SocketManager *m_socketManager;
    QDateTime m_lastUpdateTime;
};
