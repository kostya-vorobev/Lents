#ifndef CHATCLASS_H
#define CHATCLASS_H

#include <QString>
#include <QDateTime>

class ChatClass
{
public:
    ChatClass(int id, int user1_id, QString user1_name, int user2_id, QString user2_name, QString message, QDateTime sent_at);

    int getId() ;
    void setId(int id);

    int getUser1Id() ;
    void setUser1Id(int user1_id);

    QString getUser1Name() ;
    void setUser1Name(QString user1_name);

    int getUser2Id() ;
    void setUser2Id(int user2_id);

    QString getUser2Name() ;
    void setUser2Name(QString user2_name);

    QString getMessage() ;
    void setMessage(QString message);

    QDateTime getSentAt() ;
    void setSentAt(QDateTime sent_at);

private:
    int id;
    int user1_id;
    QString user1_name;
    int user2_id;
    QString user2_name;
    QString message;
    QDateTime sent_at;
};

#endif // CHATCLASS_H
