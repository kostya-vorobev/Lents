#ifndef POSTCLASS_H
#define POSTCLASS_H
#include <string>

class PostClass
{
protected:
    std::string author;
    std::string postText;
    int countLike;
    int countDislike;
public:
    PostClass(const std::string& author, const std::string& postText, int countLike, int countDislike);
    // Геттеры
    std::string getAuthor();
    std::string getPostText();
    int getCountLike();
    int getCountDislike();

    // Сеттеры
    void setAuthor(const std::string& author);
    void setPostText(const std::string& postText);
    void setCountLike(int countLike);
    void setCountDislike(int countDislike);

    // Методы для увеличения/уменьшения количества лайков и дизлайков
    void incrementLikes();
    void decrementLikes();
    void incrementDislikes();
    void decrementDislikes();

};

#endif // POSTCLASS_H
