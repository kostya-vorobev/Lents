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
#include <socketmanager.h>

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

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();
    //chat
    void onReadyRead(const QByteArray &receivedData);
    void on_SearchChatTB_textChanged();
    void onConnected();
    void checkAndUpdateChats();

private:
    Ui::MainWindow *ui;
    void addPost(const QString& author, const QString& text, int likes, int comments, QVBoxLayout* layout);
    void updateLayout(QWidget* widget);
    //chat
    SocketManager *socketManager;
    QString m_username;
    QTimer *m_timer = new QTimer(this);
    void setupConnection();
    void requestUserChats();
    void updateChatLayout(QWidget *widget);
    UserChatButton* createChatButton(const QString &chatName, const QString &lastMessage, const QDateTime &messageTime);
    void updateSearch(const QString &newText);
};
#endif // MAINWINDOW_H
