#include <QMainWindow>
#include <QVBoxLayout>
#include "sendmessageuser.h"
#include <QQueue>
#include <QTimer>
#include <QScrollBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <networkmanager.h>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <chatclass.h>

namespace Ui
{
class QDialogUsers;
}

class QDialogUsers : public QMainWindow
{
    Q_OBJECT

public:
    explicit QDialogUsers(ChatClass* chatUnit, QWidget *parent = nullptr);
    ~QDialogUsers();

private slots:
    void on_pushButton_clicked();
    void on_newDataReceived(const QJsonArray &jsonArray);
    void requestUpdatedData();
    void onHttpFinished(QNetworkReply *reply);
    //void onReadyRead(const QJsonArray &jsonArray);

private:
    void sendToServer(const QString& message, const QString& sender, const QString& receiver);

    QString m_username;
    QString m_usernameAuth;
    QQueue<QByteArray> m_writeQueue;
    Ui::QDialogUsers *ui;
    QDateTime m_lastUpdateTime;
    NetworkManager *networkManager;
    QMap<QDateTime, QPair<QString, QString>> maps;
    ChatClass* chatUnit;

};
