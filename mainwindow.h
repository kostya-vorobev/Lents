#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <postclass.h>
#include <string>
#include <lentelem.h>
#include <newselem.h>
//chat
#include <userchatbutton.h>
#include <QByteArray>
#include <QString>
#include <QScrollArea>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QTimer>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <networkmanager.h>
#include <QMultiMap>
#include <QListWidget>
#include <chatclass.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendCommand(const QString& command, const QString& message);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();
    //chat
    void checkAndUpdateChats();
    void on_pushButton_5_clicked();
    void onHttpFinished(QNetworkReply *reply);
    void onReadyRead(const QJsonArray &jsonArray);
    void onReadyReadPosts(const QJsonArray &jsonArray);

    void on_updatePostB_clicked();

private:
    Ui::MainWindow *ui;
    void addPost(int postID, const QString& author, const QString& text, int likes, int disLikes, QVBoxLayout* layout);
    void updateLayout(QWidget* widget, QWidget* page);
    //chat
    NetworkManager *networkManager;
    int userID;
    QString m_username;
    QTimer *m_timer;
    void setupConnection();
    void requestUserChats();
    void requestUserPosts();
    void updateChatLayout(QWidget *widget);
    UserChatButton* createChatButton(ChatClass* chatUnit);
    void updateSearch(const QString &newText);
    QWidget *LentPage;
    QWidget *CreatePostPage;
    QWidget *TestsPage;
    QWidget *ChatPage;
    QListWidget *list;
};
#endif // MAINWINDOW_H
