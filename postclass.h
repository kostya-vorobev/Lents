#ifndef POSTCLASS_H
#define POSTCLASS_H
#include <string>
#include <QString>

class PostClass
{
protected:
    QString author;
    QString postText;
    int countLike;
    int countDislike;
    int postID;
public:
    PostClass(int postID, const QString& author, const QString& postText, int countLike, int countDislike);
    // Геттеры
    QString getAuthor();
    QString getPostText();
    int getCountLike();
    int getCountDislike();
    int getID();

    // Сеттеры
    void setAuthor(const QString& author);
    void setPostText(const QString& postText);
    void setCountLike(int countLike);
    void setCountDislike(int countDislike);
    void setID(int postID);

    // Методы для увеличения/уменьшения количества лайков и дизлайков
    void incrementLikes();
    void decrementLikes();
    void incrementDislikes();
    void decrementDislikes();

};

#endif // POSTCLASS_H
